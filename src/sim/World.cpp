#include "World.h"

#include <cmath>

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

    while (true) {
        // In order to ensure we're sleeping the correct amount of time, we time
        // the drawing operation and take it into account when we sleep.
        double start(sim::getHighResTime());

        // For now, once our robot has collided, there's no recovery
        if (!m_collision) {
            // Update the position of the mouse and check for collisions
            m_mouse->update(Seconds(P()->deltaTime()));
            checkCollision();
        }

        // Get the duration of the drawing operation, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(sim::getHighResTime());
        double duration = end - start;

        // Sleep the appropriate amout of time, base on the drawing duration
        sleep(Seconds(std::max(0.0, P()->deltaTime() - duration)));
    }
}

std::vector<Tile*> World::getTilesContainingMouse() {

    // Select an arbitrary point belonging to the mouse
    Cartesian point = m_mouse->getShapes().at(0).getVertices().at(0);
    int px = static_cast<int>(floor((point.getX() / (P()->wallLength() + P()->wallWidth())).getMeters()));
    int py = static_cast<int>(floor((point.getY() / (P()->wallLength() + P()->wallWidth())).getMeters()));

    // Find all of the tiles that the mouse could be contained within
    std::vector<Tile*> tilesContainingMouse;
    for (int x = -1; x <= 1; x += 1) {
        for (int y = -1; y <= 1; y += 1) {
            if (0 <= px + x && px + x < m_maze->getWidth() && 0 <= y && y <= m_maze->getHeight()) {
                tilesContainingMouse.push_back(m_maze->getTile(px + x, py + y));
            }
        }
    }

    return tilesContainingMouse;
}

void World::checkCollision() {

    // For each tile the mouse could be in...
    for (Tile* tile : getTilesContainingMouse()) {

        // ...iterate through all mouse polygons...
        for (Polygon mousePolygon : m_mouse->getShapes()) {
            std::vector<Cartesian> mousePoints(mousePolygon.getVertices());
            Cartesian previousMousePoint = mousePoints.back();
            for (Cartesian currentMousePoint : mousePoints) {
                std::pair<Cartesian, Cartesian> A = std::make_pair(previousMousePoint, currentMousePoint);

                // ... and check to see if one is colliding with an obstacle polygon
                std::vector<std::vector<Polygon>> obstacleGroups = {tile->getWallPolygons(), tile->getCornerPolygons()};
                for (std::vector<Polygon> group : obstacleGroups ) {
                    for (Polygon obstacle : group) {
                        Cartesian previousObstaclePoint = obstacle.getVertices().back();
                        for (Cartesian currentObstaclePoint : obstacle.getVertices()) {
                            std::pair<Cartesian, Cartesian> B = std::make_pair(previousObstaclePoint, currentObstaclePoint);
                            if (linesIntersect(A, B)) {
                                m_collision = true;
                                return;
                            }
                            previousObstaclePoint = currentObstaclePoint;
                        } 
                    }
                }
                previousMousePoint = currentMousePoint;
            }
        }
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
