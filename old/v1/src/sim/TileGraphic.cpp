#include "TileGraphic.h"

#include "Constants.h"
#include "GraphicsTools.h"
#include "Parameters.h"

namespace sim{

TileGraphic::TileGraphic(Tile* tile) : m_tile(tile), m_color(BLACK)
{ }

TileGraphic::~TileGraphic()
{ }

void TileGraphic::draw(){

    int tileX = m_tile->getX();
    int tileY = m_tile->getY();

    int tileLength = UNITS_PER_TILE * PIXELS_PER_UNIT;

    // Lower left corner
    float c1X = tileLength * tileX;
    float c1Y = tileLength * tileY;
    
    // Upper right corner
    float c2X = tileLength * (tileX+1);
    float c2Y = tileLength * (tileY+1);

    // Update the color to reflect the number of passes
    GLfloat intensity = (float)(.1*m_tile->getPasses());
    GLfloat COLOR[3] = {0, intensity, intensity};
    m_color = COLOR;

    drawRect(c1X, c1Y, c2X, c2Y, m_color);
    if (m_tile->getPosp()){ // Draw red rectangle if it's posp
        int d = 4*PIXELS_PER_UNIT; // Distnace inward to draw rectangle
        drawRect(c1X+d, c1Y+d, c2X-d, c2Y-d, RED);
    }
    drawCorners(c1X, c1Y, c2X, c2Y);
    drawWalls(c1X, c1Y, c2X, c2Y);
}

void TileGraphic::drawRect(float c1X, float c1Y, float c2X, float c2Y, GLfloat* color){

    c1X = convertHorizontalPoint(c1X);
    c1Y = convertVerticalPoint(c1Y);
    c2X = convertHorizontalPoint(c2X);
    c2Y = convertVerticalPoint(c2Y);

    glColor3fv(color);
    glBegin(GL_POLYGON);
        glVertex2f(c1X, c1Y);
        glVertex2f(c1X, c2Y);
        glVertex2f(c2X, c2Y);
        glVertex2f(c2X, c1Y);
    glEnd();
}

void TileGraphic::drawCorners(float c1X, float c1Y, float c2X, float c2Y){

    // By definition, corners and walls are one unit of length

    // Bottem left corner
    drawRect(c1X, c1Y, c1X + PIXELS_PER_UNIT, c1Y + PIXELS_PER_UNIT, GRAY);
    // Bottem right corner
    drawRect(c2X - PIXELS_PER_UNIT, c1Y, c2X, c1Y + PIXELS_PER_UNIT, GRAY);
    // Upper left corner
    drawRect(c1X, c2Y - PIXELS_PER_UNIT, c1X + PIXELS_PER_UNIT, c2Y, GRAY);
    // Upper right corner
    drawRect(c2X - PIXELS_PER_UNIT, c2Y - PIXELS_PER_UNIT, c2X, c2Y, GRAY);

}

void TileGraphic::drawWalls(float c1X, float c1Y, float c2X, float c2Y){

    // By definition, corners and walls are one unit of length

    if (m_tile->isWall(NORTH)){
        drawRect(c1X + PIXELS_PER_UNIT, c2Y - PIXELS_PER_UNIT, c2X - PIXELS_PER_UNIT, c2Y, GRAY);
    }
    if (m_tile->isWall(EAST)){
        drawRect(c2X - PIXELS_PER_UNIT, c1Y + PIXELS_PER_UNIT, c2X, c2Y - PIXELS_PER_UNIT, GRAY);
    }
    if (m_tile->isWall(SOUTH)){
        drawRect(c1X + PIXELS_PER_UNIT, c1Y, c2X - PIXELS_PER_UNIT, c1Y + PIXELS_PER_UNIT, GRAY);
    }
    if (m_tile->isWall(WEST)){
        drawRect(c1X, c1Y + PIXELS_PER_UNIT, c1X + PIXELS_PER_UNIT, c2Y - PIXELS_PER_UNIT, GRAY);
    }
}

} // namespace sim
