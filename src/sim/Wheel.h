#pragma once

#include <Cartesian.h>
#include <Meters.h>
#include <Radians.h>
#include <RadiansPerSecond.h>

namespace sim {

// The wheel class is a combination of both the motors and wheels
class Wheel {

// The position is the starting position of center of the wheel
// For now, we assume that all wheels are vertical
public:
    Wheel(); // TODO: Necessary because we delay creation of a wheel in the mouse class... hmmm
             // TODO: Can we possible write a method in the mouse class that returns a wheel, and use a copy constructor?
    Wheel(const Distance& radius, const Distance& width, const Coordinate& position);
    Meters getRadius() const;
    Meters getWidth() const;
    Cartesian getPosition() const;

    // DC motor methods
    RadiansPerSecond getAngularVelocity() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);

    // TODO: These will be for Stepper Motors
    void rotate(const Angle& angle);
    Radians popRotation(); // Retrieves and clears

private:
    // Constants (wheel dimensions)
    Meters m_radius;
    Meters m_width;
    Cartesian m_position;

    // Variables (rotation information)
    Radians m_rotation;
    RadiansPerSecond m_angularVelocity;
};

} // namespace sim
