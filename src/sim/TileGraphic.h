#pragma once

#include "Color.h"
#include "Tile.h"

namespace sim {

class TileGraphic {

public:

    TileGraphic(const Tile* tile);

    bool wallDeclared(Direction direction) const;

    void setColor(const Color color);
    void setDistance(int distance);
    void setFogginess(bool foggy);
    void declareWall(Direction direction, bool isWall);
    void undeclareWall(Direction direction);

    void draw() const;
    void updateColor() const;
    void updateDistance() const;
    void updateFog() const;
    void updateWalls() const;
    
private:
    const Tile* m_tile;
    Color m_color;
    int m_distance;
    bool m_foggy;
    std::map<Direction, bool> m_declaredWalls;

    void updateWall(Direction direction) const;
    std::pair<Color, float> deduceWallColorAndAlpha(Direction direction) const;
    Polygon getTextPolygon(int numRows, int numCols, int row, int col) const;
};

} // namespace sim
