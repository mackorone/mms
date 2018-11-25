#include "Mouse.h"

#include <QMutableMapIterator>
#include <QPair>
#include <QVector>
#include <QtMath>

#include "units/Distance.h"

#include "Assert.h"
#include "Dimensions.h"
#include "GeometryUtilities.h"

namespace mms {

Mouse::Mouse(const Maze* maze) :
    m_maze(maze),
    m_crashed(false) {

    // The initial translation of the mouse is just the center of the starting tile
    Distance halfOfTileDistance = Dimensions::halfTileLength();
    m_initialTranslation = Coordinate::Cartesian(halfOfTileDistance, halfOfTileDistance);
    m_currentTranslation = m_initialTranslation;

    // The initial rotation of the mouse is determined by the starting tile walls
    Direction optimalStartingDirection = Direction::NORTH;
    m_startedDirection = optimalStartingDirection;
    m_startingDirection = m_startedDirection;
    m_initialRotation = DIRECTION_TO_ANGLE().value(m_startingDirection);
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

bool Mouse::didCrash() const {
    return m_crashed;
}

void Mouse::setCrashed() {
    m_crashed = true;
}

void Mouse::reset() {
    teleport(
        getInitialTranslation(),
        DIRECTION_TO_ANGLE().value(m_startingDirection)
    );
    m_startedDirection = m_startingDirection;
    m_crashed = false;
}

void Mouse::teleport(const Coordinate& translation, const Angle& rotation) {
    m_currentTranslation = translation;
    m_currentRotation = rotation;
}

Direction Mouse::getStartedDirection() const {
    return m_startedDirection;
}

void Mouse::setStartingDirection(Direction direction) {
    m_startingDirection = direction;
}

const Coordinate& Mouse::getInitialTranslation() const {
    return m_initialTranslation;
}

const Coordinate& Mouse::getCurrentTranslation() const {
    return m_currentTranslation;
}

const Angle& Mouse::getCurrentRotation() const {
    return m_currentRotation;
}

QPair<int, int> Mouse::getCurrentDiscretizedTranslation() const {
    static Distance tileLength = Dimensions::tileLength();
    Coordinate currentTranslation = getCurrentTranslation();
    int x = static_cast<int>(qFloor(currentTranslation.getX() / tileLength));
    int y = static_cast<int>(qFloor(currentTranslation.getY() / tileLength));
    return {x, y};
}

Direction Mouse::getCurrentDiscretizedRotation() const {
    int dir = static_cast<int>(qFloor(
        (getCurrentRotation() + Angle::Degrees(45)).getRadiansZeroTo2pi() /
        Angle::Degrees(90).getRadiansZeroTo2pi()
    ));
    switch (dir) {
        case 0:
            return Direction::EAST;
        case 1:
            return Direction::NORTH;
        case 2:
            return Direction::WEST;
        case 3:
            return Direction::SOUTH;
        default:
            ASSERT_NEVER_RUNS();
    }
}

Polygon Mouse::getCurrentBodyPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return getCurrentPolygon(
        m_initialBodyPolygon,
        currentTranslation,
        currentRotation);
}

Polygon Mouse::getCurrentWheelPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return getCurrentPolygon(
        m_initialWheelPolygon,
        currentTranslation,
        currentRotation
    );
}

Polygon Mouse::getCurrentPolygon(
        const Polygon& initialPolygon,
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {
    return initialPolygon
        .translate(currentTranslation - getInitialTranslation())
        .rotateAroundPoint(currentRotation - m_initialRotation, currentTranslation);
}

} 
