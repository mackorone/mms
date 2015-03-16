#include "World.h"

#include <cmath>
#include <thread>

#include <Centimeters.h>
#include <RadiansPerSecond.h>
#include <Milliseconds.h>

#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"

namespace sim {

World::World(Maze* maze, Mouse* mouse) : m_maze(maze), m_mouse(mouse), m_collision(false) {
}

void World::simulate() {

    // Start a separate collision detection thread
    std::thread collisionDetector(&World::checkCollision, this);

    // Use this thread to perform mouse position updates
    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the mouse position update operation and take it into account when we sleep.
        double start(sim::getHighResTime());

        // Update the position of the mouse if we haven't collided
        if (!m_collision) {
            m_mouse->update(Seconds(1.0/P()->mousePositionUpdateRate()));
        }

        // Get the duration of the mouse position update, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(sim::getHighResTime());
        double duration = end - start;

        // Notify the use of a late mouse position update
        if (P()->printLateMousePositionUpdates() && duration > 1.0/sim::P()->mousePositionUpdateRate()) {
            sim::print(std::string("A mouse position update was late by ")
                + std::to_string(duration - 1.0/sim::P()->mousePositionUpdateRate())
                + std::string(" seconds, which is ")
                + std::to_string((duration - 1.0/P()->mousePositionUpdateRate())/(1.0/P()->mousePositionUpdateRate()) * 100)
                + std::string(" percent late."));
        }

        // Sleep the appropriate amout of time, based on the mouse update duration
        sim::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->mousePositionUpdateRate() - duration)));
    }
}

const Tile* World::getTileContainingPoint(const Cartesian& point) {
    int x = static_cast<int>(floor((point.getX() / (P()->wallLength() + P()->wallWidth())).getMeters()));
    int y = static_cast<int>(floor((point.getY() / (P()->wallLength() + P()->wallWidth())).getMeters()));
    return m_maze->getTile(x, y);
}

void World::checkCollision() {

    while (true) {

        // In order to ensure we're sleeping the correct amount of time, we time
        // the collision detection operation and take it into account when we sleep.
        double start(sim::getHighResTime());

        // For each line segment in the mouse polygon ...
        for (std::pair<Cartesian, Cartesian> B : getLineSegments(m_mouse->getCollisionPolygon())) {

            // ... and for each tile the segment could be intersecting with ...
            const Tile* one = getTileContainingPoint(B.first);
            const Tile* two = getTileContainingPoint(B.second);
            for (const Tile* tile : {one, two}) {

                // ... iterate through all of the tile's polygons ...
                for (std::vector<Polygon> group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
                    for (Polygon obstacle : group) {
                        for (std::pair<Cartesian, Cartesian> A : getLineSegments(obstacle)) {

                            // ... and check for a collision
                            if (linesIntersect(A, B)) {
                                m_collision = true;
                                return;
                            }
                        } 
                    }
                }
            }
        }

        // Get the duration of the collision detection, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(sim::getHighResTime());
        double duration = end - start;

        // Notify the use of a late collision detection
        if (P()->printLateCollisionDetections() && duration > 1.0/sim::P()->collisionDetectionRate()) {
            sim::print(std::string("A collision detection was late by ")
                + std::to_string(duration - 1.0/sim::P()->collisionDetectionRate())
                + std::string(" seconds, which is ")
                + std::to_string((duration - 1.0/P()->collisionDetectionRate())/(1.0/P()->collisionDetectionRate()) * 100)
                + std::string(" percent late."));
        }

        // Sleep the appropriate amout of time, based on the collision detection duration
        sim::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->collisionDetectionRate() - duration)));
    }
}

#if(0) // TODO
bool Mouse::inGoal() {

    // The goal is defined to be the center of the maze 
    // This means that it's 2 squares of length if even, 1 if odd
    
    /*
    bool horizontal = (m_maze->getWidth() - 1) / 2 == getX();
    if (m_maze->getWidth() % 2 == 0) {
        horizontal = horizontal || (m_maze->getWidth()) / 2 == getX();
    }

    bool vertical = (m_maze->getHeight() - 1) / 2 == getY();
    if (m_maze->getHeight() % 2 == 0) {
        vertical = vertical || (m_maze->getHeight()) / 2 == getY();
    }

    return horizontal && vertical;
    */
    return false;
}

void Mouse::resetPosition() {
    //m_xPos = 0;
    //m_yPos = 0;
    //m_mouseDirection = NORTH;
}

void Mouse::resetColors(int curX, int curY) {
    //m_maze->resetColors(curX, curY);
}
#endif

} // namespace sim
