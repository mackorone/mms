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

    Polygon getFullPolygon() const;
    Polygon getInteriorPolygon() const;
    std::vector<Polygon> getAllWallPolygons() const;
    std::vector<Polygon> getActualWallPolygons() const;
    std::vector<Polygon> getCornerPolygons() const;
    void initPolygons();

private:
    int m_x;
    int m_y;
    std::map<Direction, bool> m_walls;

    Polygon m_fullPolygon;
    Polygon m_interiorPolygon;
    std::vector<Polygon> m_allWallPolygons;
    std::vector<Polygon> m_actualWallPolygons;
    std::vector<Polygon> m_cornerPolygons;

    void initFullPolygon();
    void initInteriorPolygon();
    void initWallPolygons();
    void initCornerPolygons();
};

} // namespace sim
