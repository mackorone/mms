#pragma once

#include <Degrees.h>
#include <Cartesian.h>
#include <Meters.h>

#include "Polygon.h"

namespace sim {

class Sensor {

public:
    Sensor();
    Cartesian getPosition() const;
    Polygon getShapes() const; // TODO: Should this function return the original position or the new positions?
    float read();

private:
    Polygon m_body;
    Cartesian m_direction;

    Meters m_range;
    Degrees m_halfWidth;
};

} // namespace sim
