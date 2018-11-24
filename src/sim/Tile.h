#pragma once

#include <QMap>
#include <QVector>

#include "Direction.h"
#include "Polygon.h"

namespace mms {

class Tile {

public:
    Tile();

    int getX() const;
    int getY() const;
    void setPos(int x, int y);

    bool isWall(Direction direction) const;
    void setWall(Direction direction, bool isWall);

    int getDistance() const;
    void setDistance(int distance);

    Polygon getFullPolygon() const;
    Polygon getInteriorPolygon() const;
    Polygon getWallPolygon(Direction direction) const;
    QVector<Polygon> getCornerPolygons() const;

    void initPolygons(int mazeWidth, int mazeHeight);

private:
    int m_x;
    int m_y;
    QMap<Direction, bool> m_walls;
    int m_distance;

    Polygon m_fullPolygon;
    Polygon m_interiorPolygon;
    QMap<Direction, Polygon> m_wallPolygons;
    QVector<Polygon> m_cornerPolygons;

    void initFullPolygon(int mazeWidth, int mazeHeight);
    void initInteriorPolygon(int mazeWidth, int mazeHeight);
    void initWallPolygons(int mazeWWidth, int mazeHeight);
    void initCornerPolygons(int mazeWWidth, int mazeHeight);
};

} 
