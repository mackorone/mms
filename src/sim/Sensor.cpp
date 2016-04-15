#include "Sensor.h"

#include <algorithm>

#include "units/Polar.h"

#include "CPMath.h"
#include "GeometryUtilities.h"
#include "Param.h"

namespace sim {

Sensor::Sensor(
        const Distance& radius,
        const Distance& range,
        const Angle& halfWidth,
        const Coordinate& position,
        const Angle& direction,
        const Maze& maze) :
        m_range(range),
        m_halfWidth(halfWidth),
        m_initialPosition(position),
        m_initialDirection(direction) {

    // Create the polygon for the body of the sensor
    m_initialPolygon = GeometryUtilities::createCirclePolygon(
        position, radius, P()->numberOfCircleApproximationPoints());

    // Create the polygon for the view of the sensor
    std::vector<Cartesian> view;
    view.push_back(position);
    for (double i = -1; i <= 1; i += 2.0 / (P()->numberOfSensorEdgePoints() - 1)) {
        view.push_back(Polar(range, (Radians(halfWidth) * i) + direction) + position);
    }
    m_initialViewPolygon = Polygon(view);

    // Initialize the sensor reading
    updateReading(m_initialPosition, m_initialDirection, maze);
}

Cartesian Sensor::getInitialPosition() const {
    return m_initialPosition;
}

Radians Sensor::getInitialDirection() const {
    return m_initialDirection;
}

const Polygon& Sensor::getInitialPolygon() const {
    return m_initialPolygon;
}

const Polygon& Sensor::getInitialViewPolygon() const {
    return m_initialViewPolygon;
}

Polygon Sensor::getCurrentViewPolygon(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) const {
    return getViewPolygon(currentPosition, currentDirection, maze);
}

double Sensor::read() const {
    return m_currentReading;
}

void Sensor::updateReading(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) {

    m_currentReading = std::max(
        0.0,
        1.0 -
            getViewPolygon(currentPosition, currentDirection, maze).area() /
            getInitialViewPolygon().area());

    SIM_ASSERT_LE(0.0, m_currentReading);
    SIM_ASSERT_LE(m_currentReading, 1.0);
}

Polygon Sensor::getViewPolygon(
        const Cartesian& currentPosition,
        const Radians& currentDirection,
        const Maze& maze) const {

    // Calling this function causes triangulation of a polygon

    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

    std::vector<Cartesian> polygon {currentPosition};

    for (double i = -1; i <= 1; i += 2.0 / (P()->numberOfSensorEdgePoints() - 1)) {
        polygon.push_back(
            GeometryUtilities::castRay(
                currentPosition,
                currentPosition + Polar(m_range, currentDirection + (m_halfWidth * i)),
                maze,
                halfWallWidth,
                tileLength
            )
        );
    }

    return Polygon(polygon);
}

} // namespace sim
