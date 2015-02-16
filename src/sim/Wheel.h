#pragma once

#include "units/Cartesian.h"
#include "units/Meters.h"
#include "units/Radians.h"
#include "units/RadiansPerSecond.h"

namespace sim {

// The wheel class is a combination of both the motors and wheels
class Wheel {

// The position is the starting position of center of the wheel
// For now, we assume that all wheels are vertical
public:
    Wheel(const Distance& radius, const Distance& width, const Coordinate& position);
    Meters getRadius() const;
    Meters getWidth() const;
    Cartesian getPosition() const; // TODO

    // These are for DC Motors
    RadiansPerSecond getAngularVelocity() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);

    // TODO
    void step();
    int getBufferedSteps();
    // TODO: These will be unused for now
    void rotate(const Angle& angle);
    Radians popRotation(); // Retrieves and clears

private:
    Meters m_radius;
    Meters m_width;
    Cartesian m_position;

    Radians m_rotation;
    RadiansPerSecond m_angularVelocity;
};

} // namespace sim
