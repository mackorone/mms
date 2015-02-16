#pragma once

#include <map>
#include <vector>

#include "Polygon.h"
#include "Directions.h"

namespace sim {

class Tile{

public:
    Tile();
    ~Tile();

    int getX();
    int getY();
    void setPos(int x, int y);

    bool isWall(Direction direction);
    void setWall(Direction direction, bool isWall);

    Polygon getBasePolygon();
    std::vector<Polygon> getWallPolygons();
    std::vector<Polygon> getCornerPolygons();

private:
    int m_x;
    int m_y;
    std::map<Direction, bool> m_walls;

    // We use this "initialized" flag to lazily initialize a tile's polygons.
    // The reason is that a tile won't know its x and y positions until after
    // it's created. However, since it must know its x and y before any of the
    // Polygon accessors will be called, we can confidently initialize when one
    // such method is actually called.
    bool m_polygonsInitialized;
    Polygon m_basePolygon;
    std::vector<Polygon> m_wallPolygons;
    std::vector<Polygon> m_cornerPolygons;

    void initPolygons();
    void initBasePolygon();
    void initWallPolygons();
    void initCornerPolygons();
};

} // namespace sim
