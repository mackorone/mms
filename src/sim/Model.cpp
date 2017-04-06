#include "Model.h"

#include <QPair>
#include <QtMath>

#include <thread>

#include "Assert.h"
#include "GeometryUtilities.h"
#include "Logging.h"
#include "Param.h"
#include "SimTime.h"
#include "SimUtilities.h"
#include "State.h"
#include "units/Milliseconds.h"

namespace mms {

Model::Model() :
    m_maze(nullptr),
    m_shutdownRequested(false) {
    ASSERT_RUNS_JUST_ONCE();
}

void Model::simulate() {

    // TODO: MACK
    // Start a separate collision detection thread
    // std::thread collisionDetector(&Model::checkCollision, this);

    // Uncomment to do mouse update benchmarking
    /*
    double start(SimUtilities::getHighResTimestamp());
    int limit = 1000;
    for (int i = 0; i < limit; i += 1) {
        m_mouse->update(Seconds(1.0 / P()->mousePositionUpdateRate()) * S()->simSpeed());
    }
    double end(SimUtilities::getHighResTimestamp());
    double duration = end - start;
    qInfo().noquote().nospace() << duration;
    SimUtilities::quit();
    */

    // Use this thread to perform mouse position updates
    while (!m_shutdownRequested) {

        // Ensure the maze/mouse aren't updated in this loop
        m_mutex.lock();

        // In order to ensure we're sleeping the correct amount of time, we time
        // the mouse position update operation and take it into account when we sleep.
        double start(SimUtilities::getHighResTimestamp());

        // If we've crashed, let this thread exit
        if (S()->crashed()) {
            // TODO: MACK
            // collisionDetector.join();
            m_mutex.unlock();
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
        SimTime::get()->incrementElapsedSimTime(elapsedSimTimeForThisIteration);

        // Update the position and stats for each mouse
        for (const QString& name : m_mice.keys()) {
            Mouse* mouse = m_mice.value(name);
            MouseStats& stats = m_stats[name];

            // Update the position of the mouse
            mouse->update(elapsedSimTimeForThisIteration);

            // Retrieve the current discretized location of the mouse, and
            // the tile at that location, for use with the next few code blocks
            QPair<int, int> location = mouse->getCurrentDiscretizedTranslation();
            const Tile* tileAtLocation = m_maze->getTile(location.first, location.second);

            // If this is a new tile, update the set of traversed tiles
            if (!stats.traversedTileLocations.contains(location)) {
                stats.traversedTileLocations.insert(location);
                if (stats.closestDistanceToCenter == -1 ||
                        tileAtLocation->getDistance() < stats.closestDistanceToCenter) {
                    stats.closestDistanceToCenter = tileAtLocation->getDistance(); 
                }
                // Alert any listeners that a new tile was entered
                emit newTileLocationTraversed(location.first, location.second);
            }

            // If we've returned to the origin, reset the departure time
            if (location.first == 0 && location.second == 0) {
                stats.timeOfOriginDeparture = Seconds(-1);
            }

            // Otherwise, if we've just left the origin, update the departure time
            else if (stats.timeOfOriginDeparture < Seconds(0)) {
                stats.timeOfOriginDeparture = SimTime::get()->elapsedSimTime();
            }

            // Separately, if we're in the center, update the best time to center
            if (m_maze->isCenterTile(location.first, location.second)) {
                Seconds timeToCenter = SimTime::get()->elapsedSimTime() - stats.timeOfOriginDeparture;
                if (stats.bestTimeToCenter < Seconds(0) || timeToCenter < stats.bestTimeToCenter) {
                    stats.bestTimeToCenter = timeToCenter;
                }
            }

            // If we're ever outside of the maze, crash. It would be cool to have
            // some "out of bounds" state but I haven't implemented that yet. We
            // continue here to make sure that we join with the other thread.
            if (!m_maze->withinMaze(location.first, location.second)) {
                S()->setCrashed();
                continue;
            }
        }

        // Release the mutex
        m_mutex.unlock();

        // Get the duration of the mouse position update, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(SimUtilities::getHighResTimestamp());
        double duration = end - start;

        // Notify the use of a late mouse position update
        if (P()->printLateMousePositionUpdates() && duration > 1.0/P()->mousePositionUpdateRate()) {
			// TODO: MACK
            // qWarning(
            //     "A mouse position update was late by %v seconds, which is %v percent late.",
            //     (duration - 1.0/P()->mousePositionUpdateRate()),
            //     (duration - 1.0/P()->mousePositionUpdateRate())/(1.0/P()->mousePositionUpdateRate()) * 100);
        }

        // Sleep the appropriate amout of time, based on the mouse update duration
        // TODO: MACK - This seems to sleep for longer than intended :/
        SimUtilities::sleep(Seconds(std::max(0.0, 1.0/P()->mousePositionUpdateRate() - duration)));
    }
}

void Model::shutdown() {
    m_shutdownRequested = true;
}

void Model::setMaze(const Maze* maze) {
    m_mutex.lock();
    m_mice.clear();
    m_stats.clear();
    m_maze = maze;
    m_mutex.unlock();
}

void Model::addMouse(const QString& name, Mouse* mouse) {
    m_mutex.lock();
    ASSERT_FA(m_maze == nullptr);
    ASSERT_FA(m_mice.contains(name));
    m_mice.insert(name, mouse);
    m_stats.insert(name, MouseStats());
    m_mutex.unlock();
}

void Model::removeMouse(const QString& name) {
    m_mutex.lock();
    ASSERT_FA(m_maze == nullptr);
    ASSERT_TR(m_mice.contains(name));
    ASSERT_TR(m_stats.contains(name));
    m_mice.remove(name);
    m_stats.remove(name);
    m_mutex.unlock();
}

bool Model::containsMouse(const QString& name) const {
    return m_mice.contains(name);
}

MouseStats Model::getMouseStats(const QString& name) const {
    ASSERT_TR(m_stats.contains(name));
    return m_stats.value(name);
}

void Model::checkCollision() {

    // If collision detectino isn't enabled, let this thread exit
    if (!P()->collisionDetectionEnabled()) {
        return;
    }

    // TODO: MACK
    // If the interface type is DISCRETE, let this thread exit
    if (true) {
        return;
    }

    /*
    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the collision detection operation and take it into account when we sleep.
        double start(mms::SimUtilities::getHighResTimestamp());

        // We declare these statically since we only need one copy of them
        static const Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
        static const Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

        // Retrieve the current collision polygon
        QVector<Cartesian> currentCollisionPolygonVertices =
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
        double end(mms::SimUtilities::getHighResTimestamp());
        double duration = end - start;

        // Notify the use of a late collision detection
        // TODO: MACK - make some variables for these long expressions
        if (P()->printLateCollisionDetections() && duration > 1.0 / P()->collisionDetectionRate()) {
            qWarning().noquote().nospace()
                << "A collision detection was late by "
                << (duration - 1.0/P()->collisionDetectionRate())
                << " seconds, which is "
                << (duration - 1.0/P()->collisionDetectionRate())/(1.0/P()->collisionDetectionRate()) * 100
                << " percent late.";
        }

        // Sleep the appropriate amout of time, based on the collision detection duration
        mms::SimUtilities::sleep(mms::Seconds(std::max(0.0, 1.0 / P()->collisionDetectionRate() - duration)));
    }
    */
}

} // namespace mms
