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
    Polygon getShapes() const;
    float read(); // TODO:.... hmmm.... should the wheels and sensors move separately from 

private:
    Cartesian m_direction;
    Cartesian m_position;
    Polygon m_polygon;

    Meters m_range;
    Degrees m_halfWidth;
};

} // namespace sim
