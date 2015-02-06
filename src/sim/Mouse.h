#pragma once

#include "Maze.h"
#include "Polygon.h"
#include "units/Cartesian.h"
#include "units/Radians.h"
#include "Wheel.h"

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

// TODO: This class should encapsulate all things to do with mouse, including sensors
// and wheels. Each of these should have a direction and position associated with them

class Mouse {

public:
    // TODO: Read in a file here???
    Mouse();

    Cartesian getTranslation() const;
    Radians getRotation() const;
    Polygon getShape() const;

private:
    Cartesian m_translation;
    Radians m_rotation;
    Polygon m_shape;

    // TODO: For now, assume that we'll have two wheels
    Wheel m_rightWheel;
    Wheel m_leftWheel;
};

} // namespace sim
