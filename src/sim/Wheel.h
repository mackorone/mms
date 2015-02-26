#pragma once

#include <Cartesian.h>
#include <Meters.h>
#include <RadiansPerSecond.h>

#include "Polygon.h"

namespace sim {

enum WheelSide { LEFT = 0, RIGHT = 1};

class Wheel {

public:
    Wheel(); // Necessary evil due to delayed wheel initialization in the mouse constructor
    Wheel(const Distance& radius, const Distance& width, const Coordinate& position);
    Meters getRadius() const;
    Meters getWidth() const;
    Cartesian getPosition() const;
    Polygon getPolygon() const;

    // DC motor methods
    RadiansPerSecond getAngularVelocity() const;
    void setAngularVelocity(const AngularVelocity& angularVelocity);
    
private:
    // Constants // TODO: Should they be though
    Meters m_radius;
    Meters m_width;
    Cartesian m_position;
    Polygon m_polygon;

    // Variables
    RadiansPerSecond m_angularVelocity;
};

} // namespace sim
