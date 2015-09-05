#pragma once

#include "Maze.h"

namespace sim{

class Mouse{

public:
    Mouse(Maze* maze);
    ~Mouse();

    // Accessor methods
    int getX();
    int getY();
    int getDirection();
    bool inGoal();

    // Action methods
    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnRight();
    void turnLeft();
    void resetPosition(); // Sets the position of the mouse to (0,0)
    void resetColors(int curX, int curY); // Resets the tile colors of the maze


private:
    Maze* m_maze;
    int m_mousePosX;
    int m_mousePosY;
    int m_mouseDirection; // NORTH, EAST, SOUTH, WEST
};

} // namespace sim
