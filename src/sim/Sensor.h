#pragma once

#include <set>

#include <Degrees.h>
#include <Cartesian.h>
#include <Meters.h>

#include "Maze.h"
#include "Polygon.h"

namespace sim {

class Sensor {

public:
    Sensor(const Coordinate& position, const Distance& radius, const Angle& rotation, const Distance& range,
           const Angle& halfWidth, const Time& readTime);
    Cartesian getInitialTranslation() const;
    Radians getInitialRotation() const;
    Polygon getInitialPolygon() const;
    Polygon getInitialView() const;
    Polygon getCurrentView(const Cartesian& currentPosition, const Radians& currentRotation, const Maze& maze) const;
    Seconds getReadTime() const;

private:
    Cartesian m_initialTranslation;
    Radians m_initialRotation;
    Polygon m_initialPolygon;

    Meters m_range;
    Degrees m_halfWidth;
    Seconds m_readTime;
    Polygon m_initialView;
};

} // namespace sim
