#pragma once

#include <set>

#include "units/Degrees.h"
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "units/Seconds.h"

#include "Maze.h"
#include "Polygon.h"

namespace sim {

class Sensor {

public:
    Sensor(
        const Distance& radius,
        const Distance& range,
        const Angle& halfWidth,
        const Duration& readDuration,
        const Coordinate& position,
        const Angle& direction);

    Seconds getReadDuration() const;
    Cartesian getInitialPosition() const;
    Radians getInitialDirection() const;
    Polygon getInitialPolygon() const;
    Polygon getInitialViewPolygon() const;
    Polygon getCurrentViewPolygon() const;
    void updateCurrentViewPolygon(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze);

private:
    Meters m_range;
    Degrees m_halfWidth;
    Seconds m_readDuration;

    Cartesian m_initialPosition;
    Radians m_initialDirection;
    Polygon m_initialPolygon;
    Polygon m_initialViewPolygon;
    Polygon m_currentViewPolygon;
};

} // namespace sim
