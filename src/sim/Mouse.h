#pragma once

#include "Maze.h"
#include "Polygon.h"
#include "units/Cartesian.h"
#include "units/RadiansPerSecond.h"
#include "Wheel.h"

#include <vector>

namespace sim {

/*
 *                               |         |
 *                               |    ^    |
 *                               |  { 0 }  |
 *                               |   ---   |
 *                               X_________|
 *
 *  As per the MicroMouse rules, the robot will always start in the lower-left
 *  corner of the maze with walls to its right, left, and rear. While, we can
 *  safely assume that a valid mouse will fit within the confines of the box, we
 *  can't assume anything else about the robot (including center of mass, wheel
 *  position, location of the robot within the box, etc.). Most all other
 *  information is left unspecified. Thus we find it convenient to choose a
 *  reference point, labeled "X" in the above diagram, from which we can
 *  determine the location of the mouse. Note that, initially, "X" is at the
 *  intersection of the middle of the left and bottom walls. If we treat the
 *  reference point "X" as if it were the mouse itself (the center of mass of
 *  which if given by "0"), then, by keeping track of both the translation and
 *  rotation of "X" (where the geometric relationship between "X" and the mouse,
 *  given by "0", is constant throughout the duration of the simulation) we can
 *  fully determine the position of the mouse. As a consequence of our choice of
 *  "X", at the beginning of the simulation, the mouse has zero translation and
 *  zero rotation - this seems to suggest that our choice of a reference point
 *  is correct. Also note that this choice of "X" ensures consistency between
 *  different mice - by pretending that "X" is the location of the mouse, all
 *  mice start in the exact same location (zero translation, zero rotation).
 *
 *  TODO: This also has to do with the fact that a rotation requires a point to
 *  TODO: rotate around. It's natural to choose the origin.
 */

// TODO: This class should encapsulate all things to do with mouse, including sensors
// and wheels. Each of these should have a direction and position associated with them

class Mouse {

public:
    // TODO: Read in a file here???
    Mouse();

    // Retrieve all shapes belonging to the mouse, including body, wheels, and sensors
    std::vector<Polygon> getShapes() const;

    // Instruct the mouse to update its own position based on how much simulation time has elapsed
    void update(const Time& elapsed);

    // TODO: Are these completely necessary???
    Wheel* getRightWheel();
    Wheel* getLeftWheel();

private:
    Cartesian getTranslation() const; // TODO: Shouldn't need

    Cartesian m_translation; // As described above, the translation of the reference point "X"
    Radians m_rotation; // Also as described above, the rotation of the reference point "X"
    Polygon m_body; // The vertices of the mouse, // TODO: Something about the initial position

    // For now, assume that we'll have two wheels
    Wheel m_rightWheel;
    Wheel m_leftWheel;
};

} // namespace sim
