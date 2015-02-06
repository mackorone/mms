#pragma once

#include "units/Cartesian.h"
#include "units/Meters.h"
#include "units/RadiansPerSecond.h"

namespace sim {

// The wheel class is a combination of both the motors and wheels
class Wheel {

// TODO: Eventually, we could include weight and rotational intertia, friction, etc...

// The position is the starting position of the wheel, from the center of the wheel, We assume that all wheels are vertical, for now
public:
    Wheel(const Distance& radius, const Distance& width, const Coordinate& position);
    Meters getRadius();
    Meters getWidth();
    Cartesian getPosition();
    RadiansPerSecond getAngularVelocity();

private:
    Meters m_radius;
    Meters m_width;
    Cartesian m_position;
    RadiansPerSecond m_angularVelocity;

};

} // namespace sim
