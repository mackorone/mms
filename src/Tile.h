#pragma once

#include <QMap>
#include <QVector>

#include "Direction.h"
#include "Polygon.h"

namespace mms {

class Tile {
 public:
  Tile();
  Tile(int x, int y, int distance, QMap<Direction, bool> walls);

  int getX() const;
  int getY() const;
  int getDistance() const;
  bool isWall(Direction direction) const;

  Polygon getFullPolygon() const;
  Polygon getWallPolygon(Direction direction) const;
  QVector<Polygon> getCornerPolygons() const;

  void initPolygons(int mazeWidth, int mazeHeight);

 private:
  int m_x;
  int m_y;
  int m_distance;
  QMap<Direction, bool> m_walls;

  Polygon m_fullPolygon;
  Polygon m_interiorPolygon;
  QMap<Direction, Polygon> m_wallPolygons;
  QVector<Polygon> m_cornerPolygons;

  void initFullPolygon(int mazeWidth, int mazeHeight);
  void initInteriorPolygon(int mazeWidth, int mazeHeight);
  void initWallPolygons();
  void initCornerPolygons();
};

}  // namespace mms
