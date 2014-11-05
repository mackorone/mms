#ifndef SIM_MOUSE_H_
#define SIM_MOUSE_H_

#include "Maze.h"
#include <vector>

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
    float getRotation();
    std::vector<std::pair<int,int>> getVertices();

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
    float m_mouseRotation; // 0 to 360 degrees
    std::vector<std::pair<int,int>> m_mouseVertices;
};

} // namespace sim

#endif // SIM_MOUSE_H_
