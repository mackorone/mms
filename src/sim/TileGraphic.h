#pragma once

#include <glut.h>

#include "Tile.h"

namespace sim {

class TileGraphic {

public:
    TileGraphic(const Tile* tile);
    void draw() const;
    void setColor(const GLfloat* color);
    bool getAlgoWall(Direction direction) const;
    void setAlgoWall(Direction direction, bool isWall);

private:
    const Tile* m_tile;
    const GLfloat* m_color;
    std::map<Direction, bool> m_algoWalls;
};

} // namespace sim
