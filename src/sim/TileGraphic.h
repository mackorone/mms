#pragma once

#include <GL/freeglut.h>

#include "Tile.h"

namespace sim {

class TileGraphic {

public:
    TileGraphic(const Tile* tile);
    void draw() const;
    void setColor(const GLfloat* color);

private:
    const Tile* m_tile;
    const GLfloat* m_color;
};

} // namespace sim
