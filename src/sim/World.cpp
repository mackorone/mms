#include "World.h"

namespace sim {

World::World(Maze* maze, Mouse* mouse) : m_maze(maze), m_mouse(mouse) {
}

void World::simulate() {
    while (true) {
        // TODO
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
