#ifndef TILEGRAPHIC_H_
#define TILEGRAPHIC_H_

#include <GL/freeglut.h>
#include "Parameters.h"
#include "Tile.h"

class TileGraphic{

public:
    TileGraphic(Tile* tile);
    ~TileGraphic();
    void draw();

private:
    Tile* m_tile;
    GLfloat* m_color;
    void drawRect(float c1X, float c1Y, float c2X, float c2Y, GLfloat* color);
    void drawCorners(float c1X, float c1Y, float c2X, float c2Y);
    void drawWalls(float c1X, float c1Y, float c2X, float c2Y);
};

#endif // TILEGRAPHIC_H_
