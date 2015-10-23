#pragma once

#include "Polygon.h"
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "units/RadiansPerSecond.h"

namespace sim {

class Wheel {

public:
    Wheel(const Distance& diameter, const Distance& width, const Coordinate& position, const Angle& direction);

    // Wheel
    Meters getDiameter() const;
    Cartesian getInitialPosition() const;
    Radians getInitialDirection() const;
    Polygon getInitialPolygon() const;

    // Motor
    RadiansPerSecond getAngularVelocity() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);

    // Encoder
    // TODO

private:

    // Wheel
    Meters m_diameter;
    Cartesian m_initialPosition;
    Radians m_initialDirection;
    Polygon m_initialPolygon;

    // Motor
    RadiansPerSecond m_angularVelocity;
};

} // namespace sim
