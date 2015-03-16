#include "World.h"

#include <cmath>

#include <Centimeters.h>
#include <RadiansPerSecond.h>
#include <Milliseconds.h>

#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"

#include <iostream> // TODO
#include <iomanip> // TODO

namespace sim {

World::World(Maze* maze, Mouse* mouse) : m_maze(maze), m_mouse(mouse), m_collision(false) {
}

void World::simulate() {

    while (true) {
        // In order to ensure we're sleeping the correct amount of time, we time
        // the drawing operation and take it into account when we sleep.
        double start(sim::getHighResTime());

        // For now, once our robot has collided, there's no recovery
        if (!m_collision) {
            // Update the position of the mouse and check for collisions
            m_mouse->update(Seconds(P()->deltaTime()));
            checkCollision();
            // TODO: This block of code needs to be sped up
        }

        // Get the duration of the drawing operation, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(sim::getHighResTime());
        double duration = end - start;

        std::cout << "Duration: " << std::fixed << std::setprecision(6) << duration << std::endl;
        if (duration > P()->deltaTime()) {
            std::cout << "Delta frame is late by " << duration - P()->deltaTime() << " seconds, which is "
                      << (duration - P()->deltaTime())/P()->deltaTime() * 100 << " percent late" << std::endl;
        }

        // Sleep the appropriate amout of time, base on the drawing duration
        sleep(Seconds(std::max(0.0, P()->deltaTime() - duration)));
    }
}

const Tile* World::getTileContainingPoint(const Cartesian& point) {
    int x = static_cast<int>(floor((point.getX() / (P()->wallLength() + P()->wallWidth())).getMeters()));
    int y = static_cast<int>(floor((point.getY() / (P()->wallLength() + P()->wallWidth())).getMeters()));
    return m_maze->getTile(x, y);
}

void World::checkCollision() {

    // Next, find all of the points of the mouse to see if they lie outside of the square
    // containing the mouse. If theres a wall, it's a collision
    int i = 0; // TODO

    // For each line segment in the mouse polygon...
    for (std::pair<Cartesian, Cartesian> B : getLineSegments(m_mouse->getCollisionPolygon())) {

        // ...for each tile the segment could be intersecting with
        const Tile* one = getTileContainingPoint(B.first);
        const Tile* two = getTileContainingPoint(B.second);
        for (const Tile* tile : {one, two}) {

            // ...iterate through all of the tile's polygons...
            for (std::vector<Polygon> group : {tile->getActualWallPolygons(), tile->getCornerPolygons()}) {
                for (Polygon obstacle : group) {
                    for (std::pair<Cartesian, Cartesian> A : getLineSegments(obstacle)) {

                        i += 1; // TODO
                        if (linesIntersect(A, B)) {
                            std::cout << i << std::endl; // TODO
                            m_collision = true;
                            return;
                        }
                    } 
                }
            }
        }
    }

    // TODO: Determining the polygon that contains the
    std::cout << i << std::endl; // TODO
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
