#pragma once

#include <mutex>
#include <vector>

#include <Cartesian.h>
#include <RadiansPerSecond.h>

#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"

namespace sim {

class Mouse {

public:
    Mouse();

    // Retrieve all shapes belonging to the mouse, including body, wheels, and sensors
    std::vector<Polygon> getBodyPolygons() const;

    // Retrieve all shapes belonging to the mouse, including body, wheels, and sensors
    std::vector<Polygon> getSensorPolygons() const;

    // Instruct the mouse to update its own position based on how much simulation time has elapsed
    void update(const Time& elapsed);

    // An atomic interface for setting the wheel speeds
    void setWheelSpeeds(const AngularVelocity& leftWheelSpeed, const AngularVelocity& rightWheelSpeed);

    // Read a sensor, and returns a value from 0.0 (completely free) to 1.0 (completely blocked)
    float read(std::string sensor);

private:
    // The mouse, as it's positioned at the start execution
    Cartesian m_initialTranslation;
    Polygon m_initialPolygon;

    // The rotation and translation of the mouse, which change throughout execution
    Radians m_rotation;
    Cartesian m_translation;

    // By assumption, the mouse is differential drive.
    Wheel m_leftWheel;
    Wheel m_rightWheel;
    std::mutex m_wheelMutex; // Ensures the wheel speeds are accessed atomically w.r.t. each other

    // The sensors on the mouse
    std::map<std::string, Sensor> m_sensors;
};

} // namespace sim
