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
           const Angle& halfWidth, const Duration& readDuration);
    Cartesian getInitialTranslation() const;
    Radians getInitialRotation() const;
    Polygon getInitialPolygon() const;
    Polygon getInitialView() const;
    Polygon getCurrentView(const Cartesian& currentPosition, const Radians& currentRotation, const Maze& maze) const;
    Seconds getReadDuration() const;

private:
    Cartesian m_initialTranslation;
    Radians m_initialRotation;
    Polygon m_initialPolygon;

    Meters m_range;
    Degrees m_halfWidth;
    Seconds m_readDuration;
    Polygon m_initialView;
};

} // namespace sim
