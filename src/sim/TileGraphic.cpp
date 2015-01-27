#include "TileGraphic.h"

#include "Constants.h"
#include "Param.h"
#include "Utilities.h"

#include <iostream> // TODO

namespace sim {

// TODO: Refactor this so that we include the physical dimensions in the class
TileGraphic::TileGraphic(Tile* tile) : m_tile(tile), m_color(BLACK)
{ }

void TileGraphic::draw() {

    // Determine the tile location
    int tileX = m_tile->getX();
    int tileY = m_tile->getY();

    // Physical length
    float tileLength = P()->wallLength() + P()->wallWidth();

    // Lower left corner
    Cartesian lowerLeft(tileX * tileLength, tileY * tileLength);
    Cartesian upperRight((tileX + 1) * tileLength, (tileY + 1) * tileLength);

    // Update the color to reflect the number of passes
    GLfloat intensity = (float)(.1*m_tile->getPasses());
    GLfloat COLOR[3] = {0, intensity, intensity};
    m_color = COLOR;

    drawRect(lowerLeft, upperRight, m_color);
    /*
    if (m_tile->getPosp()) { // Draw red rectangle if it's posp
        int d = 4*P()->pixelsPerUnit(); // Distnace inward to draw rectangle
        drawRect(c1X+d, c1Y+d, c2X-d, c2Y-d, RED);
    }
    */
    drawCorners(lowerLeft, upperRight);
    drawWalls(lowerLeft, upperRight);
}

void TileGraphic::drawRect(const Cartesian& lowerLeft, const Cartesian& upperRight, GLfloat* color) {

    float c1X = physicalToOpenGl(lowerLeft).getX();
    float c1Y = physicalToOpenGl(lowerLeft).getY();
    float c2X = physicalToOpenGl(upperRight).getX();
    float c2Y = physicalToOpenGl(upperRight).getY();

    glColor3fv(color);
    glBegin(GL_POLYGON);
        glVertex2f(c1X, c1Y);
        glVertex2f(c1X, c2Y);
        glVertex2f(c2X, c2Y);
        glVertex2f(c2X, c1Y);
    glEnd();
}

void TileGraphic::drawWalls(const Cartesian& lowerLeft, const Cartesian& upperRight) {

    // By definition, corners and walls are one unit of length

    Cartesian lowerRight(upperRight.getX(), lowerLeft.getY());
    Cartesian upperLeft(lowerLeft.getX(), upperRight.getY());
    float translationAmount = P()->wallWidth() / 2.0;

    if (m_tile->isWall(NORTH)) {
        drawRect(upperLeft + Cartesian(translationAmount, -translationAmount), upperRight + Cartesian(-translationAmount, 0), RED);
    }
    if (m_tile->isWall(EAST)) {
        drawRect(lowerRight + Cartesian(-translationAmount, translationAmount), upperRight + Cartesian(0, -translationAmount), RED);
    }
    if (m_tile->isWall(SOUTH)) {
        drawRect(lowerLeft + Cartesian(translationAmount, 0), lowerRight + Cartesian(-translationAmount, translationAmount), RED);
    }
    if (m_tile->isWall(WEST)) {
        drawRect(lowerLeft + Cartesian(0, translationAmount), upperLeft + Cartesian(translationAmount, -translationAmount), RED);
    }
}

void TileGraphic::drawCorners(const Cartesian& lowerLeft, const Cartesian& upperRight) {

    Cartesian lowerRight(upperRight.getX(), lowerLeft.getY());
    Cartesian upperLeft(lowerLeft.getX(), upperRight.getY());
    float translationAmount = P()->wallWidth() / 2.0;

    // Lower left corner
    drawRect(lowerLeft, lowerLeft + Cartesian(translationAmount, translationAmount), LIGHTGRAY);
    // Lower right corner
    drawRect(lowerRight + Cartesian(-translationAmount, 0), lowerRight + Cartesian(0, translationAmount), LIGHTGRAY);
    // Upper left corner
    drawRect(upperLeft + Cartesian(0, -translationAmount), upperLeft + Cartesian(translationAmount, 0), LIGHTGRAY);
    // Upper right corner
    drawRect(upperRight + Cartesian(-translationAmount, -translationAmount), upperRight, LIGHTGRAY);
}

} // namespace sim
