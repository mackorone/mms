#include "World.h"

#include <Centimeters.h>
#include <RadiansPerSecond.h>
#include <Milliseconds.h>

#include "GeometryUtilities.h"
#include "Param.h"
#include "SimUtilities.h"

namespace sim {

World::World(Maze* maze, Mouse* mouse) : m_maze(maze), m_mouse(mouse) {
}

void World::simulate() {

    // TODO: Explain why locks aren't completely necessary here...

    while (true) {
        // TODO: Make a wrapper function

        // In order to ensure we're sleeping the correct amount of time, we time
        // the drawing operation and take it into account when we sleep.
        double start(sim::getHighResTime());

        // Update the position of the mouse and check for collisions
        m_mouse->update(Seconds(P()->deltaTime()));
        checkCollision();

        // Get the duration of the drawing operation, in seconds. Note that this duration
        // is simply the total number of real seconds that have passed, which is exactly
        // what we want (since the framerate is perceived in real-time and not CPU time).
        double end(sim::getHighResTime());
        double duration = end - start;

        // Sleep the appropriate amout of time, base on the drawing duration
        sleep(Seconds(std::max(0.0, P()->deltaTime() - duration)));
    }
}


void World::checkCollision() {

    // Use m_mouse and m_maze to determine collisition
    bool collision = false;

    /*
    std::vector<Polygon> mousePolygons(m_mouse->getShapes());
    std::vector<Polygon> mazePolygons;
    // TODO: Get the 9 maze squares here...

    for (int x = 0; x < m_maze->getWidth(); x += 1) {
        for (int y = 0; y < m_maze->getHeight(); y += 1) {
            for (Polygon polygon : m_maze->getTile(x, y)->getWallPolygons()) {
                mazePolygons.push_back(polygon);
            }
        }
    }

    for (Polygon p1 : mousePolygons) {

        // Make the mouse segments
        std::vector<std::pair<Cartesian, Cartesian>> mouseSegments;
        Cartesian prev1(p1.getVertices().back());
        for (Cartesian c1 : p1.getVertices()) {
            mouseSegments.push_back(std::make_pair(prev1, c1));
            prev1 = c1; 
        }

        for (Polygon p2 : mazePolygons) {

            // Make the maze segments
            std::vector<std::pair<Cartesian, Cartesian>> mazeSegments;
            Cartesian prev2(p2.getVertices().back());
            for (Cartesian c2 : p2.getVertices()) {
                mazeSegments.push_back(std::make_pair(prev2, c2));
                prev2 = c2; 
            }

            for (std::pair<Cartesian, Cartesian> mouseSegment : mouseSegments) {
                for (std::pair<Cartesian, Cartesian> mazeSegment : mouseSegments) {
                    if (linesIntersect(mouseSegment, mazeSegment)) {
                        std::cout
                        << mouseSegment.first.getX().getMeters() << ","
                        << mouseSegment.first.getY().getMeters() << " "
                        << mouseSegment.second.getX().getMeters() << ","
                        << mouseSegment.second.getY().getMeters() << " "
                        collision = true;
                        break;
                    }
                }
            }
        }
    }
    */
    
    if (collision) {
        print("COLLIDED");
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


bool Mouse::wallFront() {
    // TODO: Added rounds
    /*
    return m_maze->getTile(round(m_xPos), round(m_yPos))->isWall(m_mouseDirection);
    */
    return false;
}

bool Mouse::wallRight() {
    // TODO: Added rounds
    /*
    return m_maze->getTile(round(m_xPos), round(m_yPos))->isWall((m_mouseDirection+1)%4);
    */
    return false;
}

bool Mouse::wallLeft() {
    // Modulo operations don't work for negative numbers, so we add 3 instead
    /*
    return m_maze->getTile(m_xPos, m_yPos)->isWall((m_mouseDirection+3)%4);
    */
    return false;
}

void Mouse::moveForward(float meters) {

    m_translation = m_translation + Cartesian(0, meters);
    // TODO:
    /*
    //if (!wallFront()) {
        switch(m_mouseDirection) {
            case NORTH:
                m_yPos += meters;
                break;
            case EAST:
                m_xPos += meters;
                break;
            case SOUTH:
                m_yPos -= meters;
                break;
            case WEST:
                m_xPos -= meters;
                break;
        }
        //m_maze->getTile(m_xPos, m_yPos)->incrementPasses(); // TODO: move this
    //}
    else{
        // Ideally we could communicate the fact that the mouse ran into a wall
        // from within the simulation graphic itself; I will look into this at
        // some point in the future // TODO
        std::cout << "*bump*" << std::endl;
    }
    */
}

void Mouse::turnRight() {
    //m_mouseDirection = (m_mouseDirection + 1) % 4;
}

void Mouse::turnLeft() {
    // Modulo operations don't work with subtraction
    //m_mouseDirection = (m_mouseDirection + 3) % 4;
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
