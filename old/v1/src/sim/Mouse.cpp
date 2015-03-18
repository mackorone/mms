#include <iostream>

#include "Mouse.h"
#include "Constants.h"

namespace sim{

Mouse::Mouse(Maze* maze) : m_maze(maze), m_mousePosX(0), m_mousePosY(0),
                           m_mouseDirection(NORTH)
{ }

Mouse::~Mouse()
{ }

int Mouse::getX(){
    return m_mousePosX;
}

int Mouse::getY(){
    return m_mousePosY;
}

int Mouse::getDirection(){
    return m_mouseDirection;
}

bool Mouse::inGoal(){

    // The goal is defined to be the center of the maze 
    // This means that it's 4 squares of length if even, 1 if odd
    
    bool horizontal = (m_maze->getWidth() - 1) / 2 == getX();
    if (m_maze->getWidth() % 2 == 0){
        horizontal = horizontal || (m_maze->getWidth()) / 2 == getX();
    }

    bool vertical = (m_maze->getHeight() - 1) / 2 == getY();
    if (m_maze->getHeight() % 2 == 0){
        vertical = vertical || (m_maze->getHeight()) / 2 == getY();
    }

    return horizontal && vertical;
}


bool Mouse::wallFront(){
    return m_maze->getTile(m_mousePosX, m_mousePosY)->isWall(m_mouseDirection);
}

bool Mouse::wallRight(){
    return m_maze->getTile(m_mousePosX, m_mousePosY)->isWall((m_mouseDirection+1)%4);
}

bool Mouse::wallLeft(){
    // Modulo operations don't work for negative numbers, so we add 3 instead
    return m_maze->getTile(m_mousePosX, m_mousePosY)->isWall((m_mouseDirection+3)%4);
}

void Mouse::moveForward(){
    if (!wallFront()){
        switch(m_mouseDirection){
            case NORTH:
                m_mousePosY += 1; break;
            case EAST:
                m_mousePosX += 1; break;
            case SOUTH:
                m_mousePosY -= 1; break;
            case WEST:
                m_mousePosX -= 1; break;
        }
        m_maze->getTile(m_mousePosX, m_mousePosY)->incrementPasses();
    }
    else{
        // Ideally we could communicate the fact that the mouse ran into a wall
        // from within the simulation graphic itself; I will look into this at
        // some point in the future
        std::cout << "*bump*" << std::endl;
    }
}

void Mouse::turnRight(){
    m_mouseDirection = (m_mouseDirection + 1) % 4;
}

void Mouse::turnLeft(){
    // Modulo operations don't work with subtraction
    m_mouseDirection = (m_mouseDirection + 3) % 4;
}

void Mouse::resetPosition(){
    m_mousePosX = 0;
    m_mousePosY = 0;
    m_mouseDirection = NORTH;
}

void Mouse::resetColors(int curX, int curY){
    m_maze->resetColors(curX, curY);
}

} // namespace sim
