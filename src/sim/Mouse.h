#pragma once

#include <vector>

#include <Cartesian.h>

#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"

namespace sim {

// TODO
class Mouse {

public:
    Mouse();

    // Retrieve all shapes belonging to the mouse, including body, wheels, and sensors
    std::vector<Polygon> getShapes() const;

    // Instruct the mouse to update its own position based on how much simulation time has elapsed
    void update(const Time& elapsed);

    // TODO: Are these completely necessary - make a nicer interface for these
    Wheel* getWheel(WheelSide side);

private:
    Cartesian getTranslation() const; // TODO: Shouldn't need when we replace the reference point thing

    // The non-const rotation and translation of the mouse
    Radians m_rotation;
    Cartesian m_translation;

    // The const body of the mouse, as it's positioned at the start of the maze
    Polygon m_body;

    // By assumption, the mouse is differential drive.
    Wheel m_leftWheel;
    Wheel m_rightWheel;

    // The sensors on the mouse
    std::vector<Sensor> m_sensors;
};

} // namespace sim
