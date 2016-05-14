#pragma once

#include <map>
#include <vector>

#include "Polygon.h"
#include "Direction.h"

namespace sim {

class Tile{

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
    std::vector<Polygon> getCornerPolygons() const;

    void initPolygons(int mazeWidth, int mazeHeight);

private:
    int m_x;
    int m_y;
    std::map<Direction, bool> m_walls;
    int m_distance;

    Polygon m_fullPolygon;
    Polygon m_interiorPolygon;
    std::map<Direction, Polygon> m_wallPolygons;
    std::vector<Polygon> m_cornerPolygons;

    void initFullPolygon(int mazeWidth, int mazeHeight);
    void initInteriorPolygon(int mazeWidth, int mazeHeight);
    void initWallPolygons(int mazeWWidth, int mazeHeight);
    void initCornerPolygons(int mazeWWidth, int mazeHeight);
};

} // namespace sim
