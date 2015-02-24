#pragma once

#include <Cartesian.h>
#include <Meters.h>
#include <RadiansPerSecond.h>

namespace sim {

enum WheelSide { LEFT = 0, RIGHT = 1};

class Wheel {

public:
    Wheel(); // Necessary evil due to delayed wheel initialization in the mouse constructor
    Wheel(const Distance& radius, const Distance& width, const Coordinate& position);
    Meters getRadius() const;
    Meters getWidth() const;
    Cartesian getPosition() const;

    // DC motor methods
    RadiansPerSecond getAngularVelocity() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);
    
private:
    // Note that the angle of each wheel, with respect to the mouse, is always zero.
    Meters m_radius;
    Meters m_width;

    // The initial position of the wheel, relative to the origin
    Cartesian m_position;

    // The current angular velocity of the wheel
    RadiansPerSecond m_angularVelocity;
};

} // namespace sim
