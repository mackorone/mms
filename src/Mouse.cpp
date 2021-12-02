#include "Mouse.h"

#include <QMutableMapIterator>
#include <QPair>
#include <QVector>
#include <QtMath>
#include <QMessageBox>

#include "units/Distance.h"

#include "AssertMacros.h"
#include "Dimensions.h"
#include "GeometryUtilities.h"

namespace mms {

Mouse::Mouse() {

    // The initial translation of the mouse is just the center of the starting tile
    Distance halfOfTileDistance = Dimensions::halfTileLength();
    m_initialTranslation = Coordinate::Cartesian(halfOfTileDistance, halfOfTileDistance);
    m_currentTranslation = m_initialTranslation;

    // The initial rotation of the mouse is determined by the starting tile walls
    m_initialRotation = DIRECTION_TO_ANGLE().value(Direction::NORTH);
    m_currentRotation = m_initialRotation;

    // Initialize the body, wheels, and sensors, such that they have the
    // correct initial translation and rotation
    QVector<Coordinate> bodyVertices = {
        Coordinate::Cartesian(Distance::Meters(0.00), Distance::Meters(0.00)),
        Coordinate::Cartesian(Distance::Meters(0.00), Distance::Meters(0.06)),
        Coordinate::Cartesian(Distance::Meters(0.03), Distance::Meters(0.09)),
        Coordinate::Cartesian(Distance::Meters(0.06), Distance::Meters(0.06)),
        Coordinate::Cartesian(Distance::Meters(0.06), Distance::Meters(0.00)),
    };
    Coordinate centerOfMass = Coordinate::Cartesian(
        Distance::Meters(0.06),
        Distance::Meters(0.06)
    );
    for (int i = 0; i < bodyVertices.size(); i += 1) {
        bodyVertices[i] = GeometryUtilities::translateVertex(
            bodyVertices.at(i),
            centerOfMass
        );
    }
    m_initialBodyPolygon = Polygon(bodyVertices);

    // Initialize the wheel polygon
    QVector<Coordinate> wheelVertices = {
        Coordinate::Cartesian(Distance::Meters(-0.005), Distance::Meters(0.01)),
        Coordinate::Cartesian(Distance::Meters(-0.005), Distance::Meters(0.05)),
        Coordinate::Cartesian(Distance::Meters(0.065), Distance::Meters(0.05)),
        Coordinate::Cartesian(Distance::Meters(0.065), Distance::Meters(0.01)),
    };
    for (int i = 0; i < wheelVertices.size(); i += 1) {
        wheelVertices[i] = GeometryUtilities::translateVertex(
            wheelVertices.at(i),
            centerOfMass
        );
    }
    m_initialWheelPolygon = Polygon(wheelVertices);

    // Force triangulation of the drawable polygons, thus ensuring
    // that we only triangulate once, at the beginning of execution
    m_initialBodyPolygon.getTriangles();
    m_initialWheelPolygon.getTriangles();
}

void Mouse::reset() {
    teleport(
        m_initialTranslation,
        DIRECTION_TO_ANGLE().value(Direction::NORTH)
    );
}

void Mouse::teleport(const Coordinate& translation, const Angle& rotation) {
    m_currentTranslation = translation;
    m_currentRotation = rotation;
}


QPair<int, int> Mouse::getCurrentHalfDiscretizedTranslation() const {
    static Distance halfTileLength = Dimensions::halfTileLength();
    double  tx = m_currentTranslation.getX().getMeters() * 1000.0f;
    double  ty = m_currentTranslation.getY().getMeters() * 1000.0f;
    double  hd = halfTileLength.getMeters() * 1000.0f;
    int     txi = qFloor( tx ) ;
    int     hdi = qFloor( hd )  ;
    int     tyi = qFloor( ty ) ;

    int x = static_cast<int>(qFloor(  txi / hdi ) );
    int y = static_cast<int>(qFloor(  tyi / hdi ) );

    return {x, y};
}

QPair<int, int> Mouse::getCurrentDiscretizedTranslation() const {
    static Distance tileLength = Dimensions::tileLength();
    int x = static_cast<int>(qFloor(m_currentTranslation.getX() / tileLength));
    int y = static_cast<int>(qFloor(m_currentTranslation.getY() / tileLength));
    return {x, y};
}

Direction Mouse::getCurrentDiscretizedRotation() const {
    int dir = static_cast<int>(qFloor(
        (m_currentRotation + Angle::Degrees(22.5)).getRadiansZeroTo2pi() /
        Angle::Degrees(45).getRadiansZeroTo2pi()
    ));

    switch (dir) {
        case 0:
            return Direction::EAST;
        case 2:
            return Direction::NORTH;
        case 4:
            return Direction::WEST;
        case 6:
            return Direction::SOUTH;
        case 1:
            return Direction::NORTHEAST;
        case 3:
            return Direction::NORTHWEST;
        case 5:
            return Direction::SOUTHWEST;
        case 7:
            return Direction::SOUTHEAST;
        default:
            ASSERT_NEVER_RUNS();
    }
    return Direction::NORTH; // no meaning
}

Polygon Mouse::getCurrentBodyPolygon() const {
    return getCurrentPolygon(m_initialBodyPolygon);
}

Polygon Mouse::getCurrentWheelPolygon() const {
    return getCurrentPolygon(m_initialWheelPolygon);
}

Polygon Mouse::getCurrentPolygon(const Polygon& initialPolygon) const {
    return initialPolygon
        .translate(m_currentTranslation - m_initialTranslation)
        .rotateAroundPoint(
            m_currentRotation - m_initialRotation,
            m_currentTranslation);
}

} 
