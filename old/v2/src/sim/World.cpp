#include "World.h"

#include <thread>

#include "CPMath.h"

#include "GeometryUtilities.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"
#include "units/Milliseconds.h"

namespace sim {

World::World(
        const Maze* maze,
        Mouse* mouse) :
        m_maze(maze),
        m_mouse(mouse),
        m_bestTimeToCenter(Seconds(-1)),
        m_timeOfOriginDeparture(Seconds(-1)),
        m_closestDistanceToCenter(-1) {
}

void World::setOptions(StaticMouseAlgorithmOptions options) {
    m_options = options;
}

Seconds World::getBestTimeToCenter() const {
    return m_bestTimeToCenter;
}

Seconds World::getTimeSinceOriginDeparture() const {
    // If we haven't left the origin yet, return -1
    if (m_timeOfOriginDeparture < Seconds(0)) {
        return Seconds(-1);
    }
    return T()->elapsedSimTime() - m_timeOfOriginDeparture;
}

int World::getNumberOfTilesTraversed() const {
    return m_traversedTileLocations.size();
}

int World::getClosestDistanceToCenter() const {
    return m_closestDistanceToCenter;
}

void World::simulate() {

    // Start a separate collision detection thread
    std::thread collisionDetector(&World::checkCollision, this);

    // Uncomment to do mouse update benchmarking
    /*
    double start(SimUtilities::getHighResTimestamp());
    int limit = 1000;
    for (int i = 0; i < limit; i += 1) {
        m_mouse->update(Seconds(1.0 / P()->mousePositionUpdateRate()) * S()->simSpeed());
    }
    double end(SimUtilities::getHighResTimestamp());
    double duration = end - start;
    L()->info("%v", duration);
    SimUtilities::quit();
    */

    // Use this thread to perform mouse position updates
    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the mouse position update operation and take it into account when we sleep.
        double start(SimUtilities::getHighResTimestamp());

        // If we've crashed, let this thread exit
        if (S()->crashed()) {
            collisionDetector.join();
            return;
        }

        // If the simulation is paused, simply sleep and continue
        if (S()->paused()) {
            SimUtilities::sleep(Milliseconds(P()->minSleepDuration()));
            continue;
        }

        // Calculate the amount of sim time that should pass during this iteration
        static Seconds realTimePerUpdate = Seconds(1.0 / P()->mousePositionUpdateRate());
        Seconds elapsedSimTimeForThisIteration = realTimePerUpdate * S()->simSpeed();

        // Update the sim time
        T()->incrementElapsedSimTime(elapsedSimTimeForThisIteration);

        // Update the position of the mouse
        m_mouse->update(elapsedSimTimeForThisIteration);

        // Retrieve the current discretized location of the mouse, and
        // the tile at that location, for use with the next few code blocks
        std::pair<int, int> location = m_mouse->getCurrentDiscretizedTranslation();
        const Tile* tileAtLocation = m_maze->getTile(location.first, location.second);

        // Whether or not this is a newly traversed tile
        bool newLocation = !ContainerUtilities::setContains(m_traversedTileLocations, location);

        // Update the set of traversed tiles
        if (newLocation) {
            m_traversedTileLocations.insert(location);
            if (m_closestDistanceToCenter == -1 ||
                    tileAtLocation->getDistance() < m_closestDistanceToCenter) {
                m_closestDistanceToCenter = tileAtLocation->getDistance(); 
            }
        }

        // If we've returned to the origin, reset the departure time
        if (location.first == 0 && location.second == 0) {
            if (Seconds(0) < m_timeOfOriginDeparture) {
                m_timeOfOriginDeparture = Seconds(-1);
            }
        }

        // Otherwise, if we've just left the origin, update the departure time
        else if (m_timeOfOriginDeparture < Seconds(0)) {
            m_timeOfOriginDeparture = T()->elapsedSimTime();
        }

        // Separately, if we're in the center, update the best time to center
        if (m_maze->isCenterTile(location.first, location.second)) {
            Seconds timeToCenter = T()->elapsedSimTime() - m_timeOfOriginDeparture;
            if (m_bestTimeToCenter < Seconds(0) || timeToCenter < m_bestTimeToCenter) {
                m_bestTimeToCenter = timeToCenter;
            }
        }

        // If we're ever outside of the maze, crash. It would be cool to have
        // some "out of bounds" state but I haven't implemented that yet. We
        // continue here to make sure that we join with the other thread.
        if (!m_maze->withinMaze(location.first, location.second)) {
            S()->setCrashed();
            continue;
        }

        // Get the duration of the mouse position update, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(SimUtilities::getHighResTimestamp());
        double duration = end - start;

        // Notify the use of a late mouse position update
        if (P()->printLateMousePositionUpdates() && duration > 1.0/P()->mousePositionUpdateRate()) {
			// TODO: MACK
            /*L()->warn(
                "A mouse position update was late by %v seconds, which is %v percent late.",
                (duration - 1.0/P()->mousePositionUpdateRate()),
                (duration - 1.0/P()->mousePositionUpdateRate())/(1.0/P()->mousePositionUpdateRate()) * 100);*/
        }

        // Sleep the appropriate amout of time, based on the mouse update duration
        // TODO: MACK - This seems to sleep for longer than intended :/
        SimUtilities::sleep(Seconds(std::max(0.0, 1.0/P()->mousePositionUpdateRate() - duration)));
    }
}

void World::checkCollision() {

    // If collision detectino isn't enabled, let this thread exit
    if (!P()->collisionDetectionEnabled()) {
        return;
    }

    // If the interface type is DISCRETE, let this thread exit
    if (STRING_TO_INTERFACE_TYPE.at(m_options.interfaceType) == InterfaceType::DISCRETE) {
        return;
    }

    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the collision detection operation and take it into account when we sleep.
        double start(sim::SimUtilities::getHighResTimestamp());

        // We declare these statically since we only need one copy of them
        static const Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
        static const Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

        // Retrieve the current collision polygon
        std::vector<Cartesian> currentCollisionPolygonVertices =
            m_mouse->getCurrentCollisionPolygon(
                m_mouse->getCurrentTranslation(), m_mouse->getCurrentRotation()).getVertices();

        // Check for collisions
        for (int i = 0; i < currentCollisionPolygonVertices.size(); i += 1) {
            int j = (i + 1) % currentCollisionPolygonVertices.size();
            Cartesian v1 = currentCollisionPolygonVertices.at(i);
            Cartesian v2 = currentCollisionPolygonVertices.at(j);
            // If a wall has come between the two vertices, then we have a collision
            if (GeometryUtilities::castRay(v1, v2, *m_maze, halfWallWidth, tileLength) != v2) {
                S()->setCrashed();
                return; // If we've crashed, let this thread exit
            }
        }

        // Get the duration of the collision detection, in seconds
        double end(sim::SimUtilities::getHighResTimestamp());
        double duration = end - start;

        // Notify the use of a late collision detection
        if (P()->printLateCollisionDetections() && duration > 1.0 / P()->collisionDetectionRate()) {
            L()->warn(
                "A collision detection was late by %v seconds, which is %v percent late.",
                (duration - 1.0/P()->collisionDetectionRate()),
                (duration - 1.0/P()->collisionDetectionRate())/(1.0/P()->collisionDetectionRate()) * 100);
        }

        // Sleep the appropriate amout of time, based on the collision detection duration
        sim::SimUtilities::sleep(sim::Seconds(std::max(0.0, 1.0 / P()->collisionDetectionRate() - duration)));
    }
}

} // namespace sim
