#include "Mouse.h"

#include <QMutableMapIterator>
#include <QPair>
#include <QVector>
#include <QtMath>

#include "AssertMacros.h"
#include "Dimensions.h"
#include "GeometryUtilities.h"
#include "units/Distance.h"

namespace mms {

QPair<int, int> SemiPosition::toMazeLocation() { return {x / 2, y / 2}; }

Mouse::Mouse() {
  // The initial translation of the mouse is just the center of the starting
  // tile
  Distance halfOfTileDistance = Dimensions::halfTileLength();
  m_initialTranslation =
      Coordinate::Cartesian(halfOfTileDistance, halfOfTileDistance);
  m_currentTranslation = m_initialTranslation;

  // The initial rotation of the mouse is determined by the starting tile walls
  m_initialRotation = DIRECTION_TO_ANGLE().value(SemiDirection::NORTH);
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
  Coordinate centerOfMass =
      Coordinate::Cartesian(Distance::Meters(0.06), Distance::Meters(0.06));
  for (int i = 0; i < bodyVertices.size(); i += 1) {
    bodyVertices[i] =
        GeometryUtilities::translateVertex(bodyVertices.at(i), centerOfMass);
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
    wheelVertices[i] =
        GeometryUtilities::translateVertex(wheelVertices.at(i), centerOfMass);
  }
  m_initialWheelPolygon = Polygon(wheelVertices);

  // Force triangulation of the drawable polygons, thus ensuring
  // that we only triangulate once, at the beginning of execution
  m_initialBodyPolygon.getTriangles();
  m_initialWheelPolygon.getTriangles();
}

// TODO: upforgrabs
// The initial direction probably shouldn't be hardcoded here nor in
// constructor. Instead, it should be set by the calling code.
void Mouse::reset() {
  teleport(m_initialTranslation,
           DIRECTION_TO_ANGLE().value(SemiDirection::NORTH));
}

void Mouse::teleport(const Coordinate &translation, const Angle &rotation) {
  m_currentTranslation = translation;
  m_currentRotation = rotation;
}

SemiPosition Mouse::getCurrentDiscretizedTranslation() const {
  static Distance halfTileLength = Dimensions::halfTileLength();
  static Distance quarterTileLength = Dimensions::quarterTileLength();
  int x = static_cast<int>(qFloor(
      (m_currentTranslation.getX() + quarterTileLength) / halfTileLength));
  int y = static_cast<int>(qFloor(
      (m_currentTranslation.getY() + quarterTileLength) / halfTileLength));
  return {x, y};
}

SemiDirection Mouse::getCurrentDiscretizedRotation() const {
  int dir = static_cast<int>(
      qFloor((m_currentRotation + Angle::Degrees(22.5)).getRadiansZeroTo2pi() /
             Angle::Degrees(45).getRadiansZeroTo2pi()));
  switch (dir) {
    case 0:
      return SemiDirection::EAST;
    case 1:
      return SemiDirection::NORTHEAST;
    case 2:
      return SemiDirection::NORTH;
    case 3:
      return SemiDirection::NORTHWEST;
    case 4:
      return SemiDirection::WEST;
    case 5:
      return SemiDirection::SOUTHWEST;
    case 6:
      return SemiDirection::SOUTH;
    case 7:
      return SemiDirection::SOUTHEAST;
    default:
      ASSERT_NEVER_RUNS();
  }
}

Polygon Mouse::getCurrentBodyPolygon() const {
  return getCurrentPolygon(m_initialBodyPolygon);
}

Polygon Mouse::getCurrentWheelPolygon() const {
  return getCurrentPolygon(m_initialWheelPolygon);
}

Polygon Mouse::getCurrentPolygon(const Polygon &initialPolygon) const {
  return initialPolygon.translate(m_currentTranslation - m_initialTranslation)
      .rotateAroundPoint(m_currentRotation - m_initialRotation,
                         m_currentTranslation);
}

}  // namespace mms
