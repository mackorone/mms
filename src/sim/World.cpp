#include "World.h"

#include "Param.h"
#include "units/Centimeters.h"
#include "units/RadiansPerSecond.h"
#include "units/Milliseconds.h"
#include "SimUtilities.h"

#include <cmath> // TODO: Necessary???
#include <iostream> // TODO
#include <math.h>

namespace sim {

World::World(Maze* maze, Mouse* mouse) : m_maze(maze), m_mouse(mouse) {
}

void World::simulate() {

    // TODO: Explain why locks aren't completely necessary here...

    // TODO
    //m_mouse->getRightWheel()->setAngularVelocity(RadiansPerSecond(2*M_PI));
    //m_mouse->getLeftWheel()->setAngularVelocity(RadiansPerSecond(-2.2*M_PI));
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

    // Use m_mouse, m_maze, getPolys(), etc. to determine collisition
    bool collision = false;
    std::vector<Polygon> mouseShapes = m_mouse->getShapes();
    Cartesian refPoint = mouseShapes.at(2).getVertices().at(0); //Bottom left coordinate of left wheel
    int x = static_cast<int>(floor(refPoint.getX().getMeters()/.168));
    int y = static_cast<int>(floor(refPoint.getY().getMeters()/.168));
    
    //Check center tile
    Tile* checkTile = m_maze->getTile(x, y);
    collision = checkTileCollision(checkTile, mouseShapes);

    //Check bottom left tile
    if(x != 0 && y != 0 && !collision) {
        checkTile = m_maze->getTile(x-1, y-1);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check center left tile
    if(x != 0  && !collision) {
        checkTile = m_maze->getTile(x-1, y);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check top left tile 
    if(x != 0 && y != 15 && !collision) {
        checkTile = m_maze->getTile(x-1, y+1);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check top center tile 
    if(y != 15 && !collision) {
        checkTile = m_maze->getTile(x, y+1);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check bottom center tile
    if(y != 0 && !collision) {
        checkTile = m_maze->getTile(x, y-1);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check top right tile
    if(x != 15 && y != 15 && !collision) {
        checkTile = m_maze->getTile(x+1, y+1);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check center right tile
    if(x != 15 && !collision) {
        checkTile = m_maze->getTile(x+1, y);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    //Check bottom right tile
    if(x != 15 && y != 0 && !collision) {
        checkTile = m_maze->getTile(x+1, y-1);
        collision = checkTileCollision(checkTile, mouseShapes);
    }

    if (collision) {
        std::cout << "COLLIDED" << std::endl;
    }
    else{
        std::cout << "No Collision" << std::endl;
    }
}

bool World::checkTileCollision(Tile* tile, std::vector<Polygon> mouseShapes) {
    bool collision;
    std::vector<Polygon> wallPolys = tile->getWallPolygons();
    std::vector<Polygon> cornerPolys = tile->getCornerPolygons();
    std::vector<Cartesian> partPoints, tilePoints;
    float partStartX, partStartY, partStartTest, partEndX, partEndY, partEndTest, wallStartX, wallStartY, wallStartTest, wallEndX, wallEndY, wallEndTest;
    //Iterate through each mouse part
    for(int i=0; i<mouseShapes.size(); i++) {
        partPoints = mouseShapes.at(i).getVertices();
        //Iterate through each point(line) in specific part
        for(int j=0; j<partPoints.size(); j++) {
            partStartX = partPoints.at(j).getX().getMeters();
            partStartY = partPoints.at(j).getY().getMeters();
            partEndX = partPoints.at((j+1) % partPoints.size()).getX().getMeters();
            partEndY = partPoints.at((j+1) % partPoints.size()).getY().getMeters();
            //Check all wall polygons
            for(int k=0; k<wallPolys.size(); k++) {
                tilePoints = wallPolys.at(k).getVertices();
                //Iterate through each line in the polygon
                for(int l=0; l<tilePoints.size(); l++) { 
                    wallStartX = tilePoints.at(l).getX().getMeters();
                    wallStartY = tilePoints.at(l).getY().getMeters();
                    wallEndX = tilePoints.at((l+1) % 4).getX().getMeters();
                    wallEndY = tilePoints.at((l+1) % 4).getY().getMeters();
                    /*Test if end points of part line are on opposite sides of 
                      wall line and vice versa */
                    partStartTest = (wallEndX-wallStartX)*(partStartY-wallEndY) - (wallEndY-wallStartY)*(partStartX-wallEndX);
                    partEndTest = (wallEndX-wallStartX)*(partEndY-wallEndY) - (wallEndY-wallStartY)*(partEndX-wallEndX);
                    wallStartTest = (partEndX-partStartX)*(wallStartY-partEndY) - (partEndY-partStartY)*(wallStartX-partEndX);
                    wallEndTest = (partEndX-partStartX)*(wallEndY-partEndY) - (partEndY-partStartY)*(wallEndX-partEndX);
                    if((partStartTest*partEndTest <= 0) && (wallStartTest*wallEndTest <=0)) {
                        return true;
                    }
                } 
            }
            //Check all corner polygons
            for(int k=0; k<cornerPolys.size(); k++) {
                tilePoints = cornerPolys.at(k).getVertices();
               //Iterate through each line in the polygon
                for(int l=0; l<tilePoints.size(); l++) {
                    wallStartX = tilePoints.at(l).getX().getMeters();
                    wallStartY = tilePoints.at(l).getY().getMeters();
                    wallEndX = tilePoints.at((l+1) % 4).getX().getMeters();
                    wallEndY = tilePoints.at((l+1) % 4).getY().getMeters();
                    /*Test if end points of part line are on opposite sides of 
                      wall line and vice versa */
                    partStartTest = (wallEndX-wallStartX)*(partStartY-wallEndY) - (wallEndY-wallStartY)*(partStartX-wallEndX);
                    partEndTest = (wallEndX-wallStartX)*(partEndY-wallEndY) - (wallEndY-wallStartY)*(partEndX-wallEndX);
                    wallStartTest = (partEndX-partStartX)*(wallStartY-partEndY) - (partEndY-partStartY)*(wallStartX-partEndX);
                    wallEndTest = (partEndX-partStartX)*(wallEndY-partEndY) - (partEndY-partStartY)*(wallEndX-partEndX);
                    if((partStartTest*partEndTest <= 0) && (wallStartTest*wallEndTest <=0)) {
                        return true;
                    }
                }                
            }
        }
    }
    return false;
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
