#pragma once

#include "Maze.h"
#include "Polygon.h"
#include "units/Cartesian.h"
#include "units/Radians.h"

#include <vector>

namespace sim {

/*
    The mouse will always start in the position drawn below. We define a
    reference point for the mouse to be in the lower left corner of the box,
    given by an "X" (this X is at the intersection of the middle of the
    left and bottom walls). All of the mouse vertices are given in coordinates
    relative to that reference point. Furthermore, the mouse translation is
    simply the vector connecting the maze origin to the reference point.
    Thus, by convention, at the beginning the simulation, the mouse has
    zero translation and zero rotation.
  
     |         |
     |   |^|   |
     |  { ^ }  |
     |   ---   |
     X_________|
 */

class Mouse {

public:
    // TODO: Read in a file here???
    Mouse(Maze* maze);

    // Accessor methods
    Cartesian getTranslation() const;
    Radians getRotation() const;
    Polygon getShape() const;

    // Action methods
    // TODO: Discrete interface, being replaced
    bool inGoal();
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
    Polygon m_shape;

    // Use only within the
    //void moveTo(float xpos, float ypos);
    //void setRotation(float radians);
};

} // namespace sim
