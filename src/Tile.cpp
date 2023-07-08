#include "Tile.h"

#include "AssertMacros.h"
#include "Dimensions.h"

namespace mms {

Tile::Tile() { ASSERT_NEVER_RUNS(); }

Tile::Tile(int x, int y, int distance, QMap<Direction, bool> walls)
    : m_x(x), m_y(y), m_distance(distance), m_walls(walls) {}

int Tile::getX() const { return m_x; }

int Tile::getY() const { return m_y; }

int Tile::getDistance() const { return m_distance; }

bool Tile::isWall(Direction direction) const {
  return m_walls.value(direction);
}

Polygon Tile::getFullPolygon() const { return m_fullPolygon; }

Polygon Tile::getWallPolygon(Direction direction) const {
  return m_wallPolygons.value(direction);
}

QVector<Polygon> Tile::getCornerPolygons() const { return m_cornerPolygons; }

void Tile::initPolygons(int mazeWidth, int mazeHeight) {
  //  The polygons associated with each tile are as follows:
  //
  //      full: 05af
  //
  //      interior: 278d
  //
  //      northWall: 7698
  //      eastWall: d8be
  //      southWall: 32dc
  //      westWall: 1472
  //
  //      lowerLeftCorner: 0123
  //      upperLeftCorner: 4567
  //      upperRightCorner: 89ab
  //      lowerRightCorner: cdef
  //
  //      5---6-------------9---a
  //      |   |             |   |
  //      4---7-------------8---b
  //      |   |             |   |
  //      |   |             |   |
  //      |   |             |   |
  //      |   |             |   |
  //      |   |             |   |
  //      1---2-------------d---e
  //      |   |             |   |
  //      0---3-------------c---f

  // Order is important
  initFullPolygon(mazeWidth, mazeHeight);
  initInteriorPolygon(mazeWidth, mazeHeight);
  initWallPolygons();
  initCornerPolygons();
}

void Tile::initFullPolygon(int mazeWidth, int mazeHeight) {
  Distance halfWallWidth = Dimensions::halfWallWidth();
  Distance tileLength = Dimensions::tileLength();
  Coordinate lowerLeftPoint = Coordinate::Cartesian(
      tileLength * getX() - halfWallWidth * (getX() == 0 ? 1 : 0),
      tileLength * getY() - halfWallWidth * (getY() == 0 ? 1 : 0));
  Coordinate upperRightPoint = Coordinate::Cartesian(
      tileLength * (getX() + 1) +
          halfWallWidth * (getX() == mazeWidth - 1 ? 1 : 0),
      tileLength * (getY() + 1) +
          halfWallWidth * (getY() == mazeHeight - 1 ? 1 : 0));
  Coordinate lowerRightPoint =
      Coordinate::Cartesian(upperRightPoint.getX(), lowerLeftPoint.getY());
  Coordinate upperLeftPoint =
      Coordinate::Cartesian(lowerLeftPoint.getX(), upperRightPoint.getY());
  m_fullPolygon = Polygon({
      lowerLeftPoint,
      upperLeftPoint,
      upperRightPoint,
      lowerRightPoint,
  });
}

void Tile::initInteriorPolygon(int mazeWidth, int mazeHeight) {
  Distance halfWallWidth = Dimensions::halfWallWidth();
  Coordinate lowerLeftPoint = m_fullPolygon.getVertices().at(0);
  Coordinate upperLeftPoint = m_fullPolygon.getVertices().at(1);
  Coordinate upperRightPoint = m_fullPolygon.getVertices().at(2);
  Coordinate lowerRightPoint = m_fullPolygon.getVertices().at(3);

  m_interiorPolygon = Polygon({
      lowerLeftPoint +
          Coordinate::Cartesian(halfWallWidth * (getX() == 0 ? 2 : 1),
                                halfWallWidth * (getY() == 0 ? 2 : 1)),
      upperLeftPoint +
          Coordinate::Cartesian(
              halfWallWidth * (getX() == 0 ? 2 : 1),
              halfWallWidth * (getY() == mazeHeight - 1 ? -2 : -1)),
      upperRightPoint +
          Coordinate::Cartesian(
              halfWallWidth * (getX() == mazeWidth - 1 ? -2 : -1),
              halfWallWidth * (getY() == mazeHeight - 1 ? -2 : -1)),
      lowerRightPoint + Coordinate::Cartesian(
                            halfWallWidth * (getX() == mazeWidth - 1 ? -2 : -1),
                            halfWallWidth * (getY() == 0 ? 2 : 1)),
  });
}

void Tile::initWallPolygons() {
  Coordinate outerLowerLeftPoint = m_fullPolygon.getVertices().at(0);
  Coordinate outerUpperLeftPoint = m_fullPolygon.getVertices().at(1);
  Coordinate outerUpperRightPoint = m_fullPolygon.getVertices().at(2);
  Coordinate outerLowerRightPoint = m_fullPolygon.getVertices().at(3);

  Coordinate innerLowerLeftPoint = m_interiorPolygon.getVertices().at(0);
  Coordinate innerUpperLeftPoint = m_interiorPolygon.getVertices().at(1);
  Coordinate innerUpperRightPoint = m_interiorPolygon.getVertices().at(2);
  Coordinate innerLowerRightPoint = m_interiorPolygon.getVertices().at(3);

  QVector<Coordinate> northWall;
  northWall.append(innerUpperLeftPoint);
  northWall.append(Coordinate::Cartesian(innerUpperLeftPoint.getX(),
                                         outerUpperLeftPoint.getY()));
  northWall.append(Coordinate::Cartesian(innerUpperRightPoint.getX(),
                                         outerUpperRightPoint.getY()));
  northWall.append(innerUpperRightPoint);
  m_wallPolygons.insert(Direction::NORTH, Polygon(northWall));

  QVector<Coordinate> eastWall;
  eastWall.append(innerLowerRightPoint);
  eastWall.append(innerUpperRightPoint);
  eastWall.append(Coordinate::Cartesian(outerUpperRightPoint.getX(),
                                        innerUpperRightPoint.getY()));
  eastWall.append(Coordinate::Cartesian(outerLowerRightPoint.getX(),
                                        innerLowerRightPoint.getY()));
  m_wallPolygons.insert(Direction::EAST, Polygon(eastWall));

  QVector<Coordinate> southWall;
  southWall.append(Coordinate::Cartesian(innerLowerLeftPoint.getX(),
                                         outerLowerLeftPoint.getY()));
  southWall.append(innerLowerLeftPoint);
  southWall.append(innerLowerRightPoint);
  southWall.append(Coordinate::Cartesian(innerLowerRightPoint.getX(),
                                         outerLowerRightPoint.getY()));
  m_wallPolygons.insert(Direction::SOUTH, Polygon(southWall));

  QVector<Coordinate> westWall;
  westWall.append(Coordinate::Cartesian(outerLowerLeftPoint.getX(),
                                        innerLowerLeftPoint.getY()));
  westWall.append(Coordinate::Cartesian(outerUpperLeftPoint.getX(),
                                        innerUpperLeftPoint.getY()));
  westWall.append(innerUpperLeftPoint);
  westWall.append(innerLowerLeftPoint);
  m_wallPolygons.insert(Direction::WEST, Polygon(westWall));
}

void Tile::initCornerPolygons() {
  Coordinate outerLowerLeftPoint = m_fullPolygon.getVertices().at(0);
  Coordinate outerUpperLeftPoint = m_fullPolygon.getVertices().at(1);
  Coordinate outerUpperRightPoint = m_fullPolygon.getVertices().at(2);
  Coordinate outerLowerRightPoint = m_fullPolygon.getVertices().at(3);

  Coordinate innerLowerLeftPoint = m_interiorPolygon.getVertices().at(0);
  Coordinate innerUpperLeftPoint = m_interiorPolygon.getVertices().at(1);
  Coordinate innerUpperRightPoint = m_interiorPolygon.getVertices().at(2);
  Coordinate innerLowerRightPoint = m_interiorPolygon.getVertices().at(3);

  QVector<Coordinate> lowerLeftCorner;
  lowerLeftCorner.append(outerLowerLeftPoint);
  lowerLeftCorner.append(Coordinate::Cartesian(outerLowerLeftPoint.getX(),
                                               innerLowerLeftPoint.getY()));
  lowerLeftCorner.append(innerLowerLeftPoint);
  lowerLeftCorner.append(Coordinate::Cartesian(innerLowerLeftPoint.getX(),
                                               outerLowerLeftPoint.getY()));
  m_cornerPolygons.append(Polygon(lowerLeftCorner));

  QVector<Coordinate> upperLeftCorner;
  upperLeftCorner.append(Coordinate::Cartesian(outerUpperLeftPoint.getX(),
                                               innerUpperLeftPoint.getY()));
  upperLeftCorner.append(outerUpperLeftPoint);
  upperLeftCorner.append(Coordinate::Cartesian(innerUpperLeftPoint.getX(),
                                               outerUpperLeftPoint.getY()));
  upperLeftCorner.append(innerUpperLeftPoint);
  m_cornerPolygons.append(Polygon(upperLeftCorner));

  QVector<Coordinate> upperRightCorner;
  upperRightCorner.append(innerUpperRightPoint);
  upperRightCorner.append(Coordinate::Cartesian(innerUpperRightPoint.getX(),
                                                outerUpperRightPoint.getY()));
  upperRightCorner.append(outerUpperRightPoint);
  upperRightCorner.append(Coordinate::Cartesian(outerUpperRightPoint.getX(),
                                                innerUpperRightPoint.getY()));
  m_cornerPolygons.append(Polygon(upperRightCorner));

  QVector<Coordinate> lowerRightCorner;
  lowerRightCorner.append(Coordinate::Cartesian(innerLowerRightPoint.getX(),
                                                outerLowerRightPoint.getY()));
  lowerRightCorner.append(innerLowerRightPoint);
  lowerRightCorner.append(Coordinate::Cartesian(outerLowerRightPoint.getX(),
                                                innerLowerRightPoint.getY()));
  lowerRightCorner.append(outerLowerRightPoint);
  m_cornerPolygons.append(Polygon(lowerRightCorner));
}

}  // namespace mms
