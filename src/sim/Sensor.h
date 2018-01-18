#pragma once

#include <set>

#include "units/Angle.h"
#include "units/Coordinate.h"
#include "units/Distance.h"
#include "units/Seconds.h"

#include "Maze.h"
#include "Polygon.h"

namespace mms {

class Sensor {

public:
    Sensor();
    Sensor(
        const Distance& radius,
        const Distance& range,
        const Angle& halfWidth,
        const Coordinate& position,
        const Angle& direction,
        const Maze& maze);

    Coordinate getInitialPosition() const;
    Angle getInitialDirection() const;
    const Polygon& getInitialPolygon() const;
    const Polygon& getInitialViewPolygon() const;
    Polygon getCurrentViewPolygon(
        const Coordinate& currentPosition,
        const Angle& currentDirection,
        const Maze& maze) const;

    double read() const;
    void updateReading(
        const Coordinate& currentPosition,
        const Angle& currentDirection,
        const Maze& maze);

private:
    Distance m_range;
    Angle m_halfWidth;

    Coordinate m_initialPosition;
    Angle m_initialDirection;
    Polygon m_initialPolygon;
    Polygon m_initialViewPolygon;

    double m_currentReading;

    Polygon getViewPolygon(
        const Coordinate& currentPosition,
        const Angle& currentDirection,
        const Maze& maze) const;
};

} // namespace mms
