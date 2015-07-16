#pragma once

#include <glut.h>

#include "Tile.h"

namespace sim {

class TileGraphic {

public:

    TileGraphic(const Tile* tile);

    bool wallDeclared(Direction direction) const;

    void setColor(const GLfloat* color);
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
    const GLfloat* m_color;
    int m_distance;
    bool m_foggy;
    std::map<Direction, bool> m_declaredWalls;

    void updateWall(Direction direction) const;
    std::pair<const GLfloat*, GLfloat> deduceWallColorAndAlpha(Direction direction) const;
};

} // namespace sim
