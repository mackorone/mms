#include "Sensor.h"

#include <QVector>
#include <QtMath>

#include <algorithm>

#include "Assert.h"
#include "GeometryUtilities.h"
#include "Param.h"
#include "units/Polar.h"

namespace mms {

Sensor::Sensor() :
    m_range(Meters(0)),
    m_halfWidth(Angle::Radians(0)),
    m_initialPosition(Cartesian(Meters(0), Meters(0))),
    m_initialDirection(Angle::Radians(0)) {
}

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
    QVector<Cartesian> view;
    view.push_back(position);
    for (double i = -1; i <= 1; i += 2.0 / (P()->numberOfSensorEdgePoints() - 1)) {
        view.push_back(Polar(range, (halfWidth * i) + direction) + position);
    }
    m_initialViewPolygon = Polygon(view);

    // Initialize the sensor reading
    updateReading(m_initialPosition, m_initialDirection, maze);
}

Cartesian Sensor::getInitialPosition() const {
    return m_initialPosition;
}

Angle Sensor::getInitialDirection() const {
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
        const Angle& currentDirection,
        const Maze& maze) const {
    return getViewPolygon(currentPosition, currentDirection, maze);
}

double Sensor::read() const {
    return m_currentReading;
}

void Sensor::updateReading(
        const Cartesian& currentPosition,
        const Angle& currentDirection,
        const Maze& maze) {

    m_currentReading = std::max(
        0.0,
        1.0 - 
            getViewPolygon(currentPosition, currentDirection, maze)
                .area().getMetersSquared() /
            getInitialViewPolygon().area().getMetersSquared());

    ASSERT_LE(0.0, m_currentReading);
    ASSERT_LE(m_currentReading, 1.0);
}

Polygon Sensor::getViewPolygon(
        const Cartesian& currentPosition,
        const Angle& currentDirection,
        const Maze& maze) const {

    // TODO: MACK - this can be deduped with getCurrentViewPolygon

    // Calling this function causes triangulation of a polygon

    static Meters halfWallWidth = Meters(P()->wallWidth() / 2.0);
    static Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());

    QVector<Cartesian> polygon {currentPosition};

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

} // namespace mms
