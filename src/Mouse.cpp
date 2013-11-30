#include "Mouse.h"

#include "Constants.h"
#include "Maze.h"
#include "Parameters.h"

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
    
    bool horizontal = (MAZE_WIDTH - 1) / 2 == getX(); // TODO: This should be a property of the maze
    if (MAZE_WIDTH % 2 == 0){
        horizontal = horizontal || (MAZE_WIDTH) / 2 == getX();
    }

    bool vertical = (MAZE_HEIGHT - 1) / 2 == getY();
    if (MAZE_HEIGHT % 2 == 0){
        vertical = vertical || (MAZE_HEIGHT) / 2 == getY();
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
    }
}

void Mouse::turnRight(){
    m_mouseDirection = (m_mouseDirection + 1) % 4;
}

void Mouse::turnLeft(){
    // Modulo operations don't work with subtraction
    m_mouseDirection = (m_mouseDirection + 3) % 4;
}
