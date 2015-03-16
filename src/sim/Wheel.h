#pragma once

#include <Cartesian.h>
#include <Meters.h>
#include <RadiansPerSecond.h>

#include "Polygon.h"

namespace sim {

class Wheel {

public:
    Wheel(); // Necessary evil due to delayed wheel initialization in the mouse constructor
    Wheel(const Distance& radius, const Distance& width, const Coordinate& position);
    Meters getRadius() const;
    Cartesian getInitialTranslation() const;
    Radians getInitialRotation() const;
    Polygon getInitialPolygon() const;

    // DC motor methods
    RadiansPerSecond getAngularVelocity() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);

private:
    Meters m_radius;
    Cartesian m_initialTranslation;
    Radians m_initialRotation;
    Polygon m_initialPolygon;

    // Variables
    RadiansPerSecond m_angularVelocity;
};

} // namespace sim
