#pragma once

#include <GL/freeglut.h>
#include "Tile.h"
#include "units/Cartesian.h"

namespace sim {

class TileGraphic {

public:
    TileGraphic(Tile* tile);
    void draw();

private:
    Tile* m_tile;
    GLfloat* m_color;
    void drawRect(const Cartesian& lowerLeft, const Cartesian& upperRight, GLfloat* color);
    void drawWalls(const Cartesian& lowerLeft, const Cartesian& upperRight);
    void drawCorners(const Cartesian& lowerLeft, const Cartesian& upperRight);
};

} // namespace sim
