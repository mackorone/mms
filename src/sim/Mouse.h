#pragma once

#include "Maze.h"
#include "units/Cartesian.h"
#include "units/Radians.h"

#include <vector>

namespace sim {

class Mouse {

public:
    Mouse(Maze* maze);

    // Accessor methods
    Cartesian getTranslation();
    Radians getRotation();
    bool inGoal();
    //std::vector<std::pair<int,int>> getVertices();

    // Action methods
    // TODO: Discrete interface, being replaced
    float getX() { return 0; }
    float getY() { return 0; }
    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward(float meters); // Causes the mouse to "teleport" forward
    void turnRight();
    void turnLeft();
    void resetPosition(); // Sets the position of the mouse to (0,0)
    void resetColors(int curX, int curY); // Resets the tile colors of the maze

private:
    Maze* m_maze;
    Cartesian m_translation;
    Radians m_rotation;

    //std::vector<std::pair<int,int>> m_mouseVertices;
    // Use only within the
    //void moveTo(float xpos, float ypos);
    //void setRotation(float radians);
};

} // namespace sim
