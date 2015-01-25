#pragma once

#include <GL/freeglut.h>
#include "Tile.h"
#include "Point.h"

namespace sim {

class TileGraphic {

public:
    TileGraphic(Tile* tile);
    void draw();

private:
    Tile* m_tile;
    GLfloat* m_color;
    void drawRect(const Point& lowerLeft, const Point& upperRight, GLfloat* color);
    void drawWalls(const Point& lowerLeft, const Point& upperRight);
    void drawCorners(const Point& lowerLeft, const Point& upperRight);
};

} // namespace sim
