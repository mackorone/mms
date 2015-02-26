#pragma once

#include <vector>

#include <Cartesian.h>

#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"

namespace sim {

class Mouse {

public:
    Mouse();

    // Retrieve all shapes belonging to the mouse, including body, wheels, and sensors
    std::vector<Polygon> getShapes() const;

    // Instruct the mouse to update its own position based on how much simulation time has elapsed
    void update(const Time& elapsed);

    // Retrieve a reference to a wheel
    Wheel* getWheel(WheelSide side);

private:
    // The rotation and translation of the mouse, which change throughout execution
    Radians m_rotation;
    Cartesian m_translation;

    // The body of the mouse, as it's positioned at the start execution
    Polygon m_body;

    // The translation of the mouse at the start of execution (depends on m_body)
    Cartesian m_start;

    // By assumption, the mouse is differential drive.
    Wheel m_leftWheel;
    Wheel m_rightWheel;

    // The sensors on the mouse
    std::vector<Sensor> m_sensors;
};

} // namespace sim
