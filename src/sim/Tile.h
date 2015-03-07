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

    int getX() const;
    int getY() const;
    void setPos(int x, int y);

    bool isWall(Direction direction);
    void setWall(Direction direction, bool isWall);

    Polygon getBasePolygon() const;
    std::vector<Polygon> getWallPolygons() const;
    std::vector<Polygon> getCornerPolygons() const;
    void initPolygons();

private:
    int m_x;
    int m_y;
    std::map<Direction, bool> m_walls;

    Polygon m_basePolygon;
    std::vector<Polygon> m_wallPolygons;
    std::vector<Polygon> m_cornerPolygons;

    void initBasePolygon();
    void initWallPolygons();
    void initCornerPolygons();
};

} // namespace sim
