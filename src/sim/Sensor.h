#pragma once

#include <Degrees.h>
#include <Cartesian.h>
#include <Meters.h>

#include "Maze.h"
#include "Polygon.h"

namespace sim {

class Sensor {

public:
    Sensor(const Coordinate& position, const Distance& radius, const Angle& rotation, const Distance& range, const Angle& halfWidth);
    float read(const Cartesian& translation, const Radians& rotation, const Maze& maze);
    Cartesian getInitialTranslation() const;
    Radians getInitialRotation() const;
    Polygon getInitialPolygon() const;
    Polygon getInitialView() const;

private:
    Cartesian m_initialTranslation;
    Radians m_initialRotation; // TODO: Might not need these.....
    Polygon m_initialPolygon;

    Meters m_range;
    Degrees m_halfWidth;
    Polygon m_initialView;
};

} // namespace sim
