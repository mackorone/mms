#include "World.h"

#include <thread>

#include "CPMath.h"

#include "GeometryUtilities.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "units/Milliseconds.h"

namespace sim {

World::World(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic,
        WorldOptions options) :
        m_maze(maze),
        m_mouse(mouse),
        m_mazeGraphic(mazeGraphic),
        m_options(options) {
}

void World::simulate() {

    // Start a separate collision detection thread
    std::thread collisionDetector(&World::checkCollision, this);

    // Uncomment to do mouse update benchmarking
    /*
    double start(SimUtilities::getHighResTime());
    int limit = 1000;
    for (int i = 0; i < limit; i += 1) {
        m_mouse->update(Seconds(1.0 / P()->mousePositionUpdateRate()) * S()->simSpeed());
    }
    double end(SimUtilities::getHighResTime());
    double duration = end - start;
    L()->info("%v", duration);
    SimUtilities::quit();
    */

    // Use this thread to perform mouse position updates
    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the mouse position update operation and take it into account when we sleep.
        double start(SimUtilities::getHighResTime());

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

        // Update the position of the mouse
        static Seconds realTimePerUpdate = Seconds(1.0 / P()->mousePositionUpdateRate());
        m_mouse->update(realTimePerUpdate * S()->simSpeed());

        // Update the tile fog. Note that this is a bit of a one-off case. We
        // shouldn't really put any sort of graphics-related stuff in this
        // class, as it's supposed to only be responsible for progressing the
        // simulation. However, since it'd be a bit much to create a class for
        // just this one bit of code, this is fine for now. If we end up
        // needing more graphics functionality here, it'd be wise to make a
        // separate class.
        if (m_options.automaticallyClearFog) {
            std::pair<int, int> location = m_mouse->getCurrentDiscretizedTranslation();
            m_mazeGraphic->setTileFogginess(location.first, location.second, false);
        }

        // Get the duration of the mouse position update, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(SimUtilities::getHighResTime());
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
        SimUtilities::sleep(Seconds(std::max(0.0, 1.0/P()->mousePositionUpdateRate() - duration)));
    }
}

void World::checkCollision() {

    // If collision detectino isn't enabled, let this thread exit
    if (!P()->collisionDetectionEnabled()) {
        return;
    }

    // If the interface type is not continuous, let this thread exit
    if (m_options.interfaceType != InterfaceType::CONTINUOUS) {
        return;
    }

    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the collision detection operation and take it into account when we sleep.
        double start(sim::SimUtilities::getHighResTime());

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
        double end(sim::SimUtilities::getHighResTime());
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
