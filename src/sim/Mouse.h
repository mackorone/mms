#ifndef MOUSE_H_
#define MOUSE_H_

#include "Maze.h"

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

private:
    Maze* m_maze;
    int m_mousePosX;
    int m_mousePosY;
    int m_mouseDirection; // NORTH, EAST, SOUTH, WEST
};

#endif // MOUSE_H_
