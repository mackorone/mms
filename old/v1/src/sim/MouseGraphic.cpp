#include "MouseGraphic.h"

#include "Constants.h"
#include "GraphicsTools.h"
#include "Mouse.h"
#include "Parameters.h"

namespace sim{

MouseGraphic::MouseGraphic(Mouse* mouse) : m_mouse(mouse), m_color(BLUE)
{ }

MouseGraphic::~MouseGraphic()
{ }

void MouseGraphic::draw(){

    int tileX = m_mouse->getX();
    int tileY = m_mouse->getY();

    int tileLength = UNITS_PER_TILE * PIXELS_PER_UNIT;

    // Lower left corner
    float c1X = (tileLength * tileX) + 2*PIXELS_PER_UNIT;
    float c1Y = (tileLength * tileY) + 2*PIXELS_PER_UNIT;
    
    // Upper right corner
    float c2X = (tileLength * (tileX+1)) - 2*PIXELS_PER_UNIT;
    float c2Y = (tileLength * (tileY+1)) - 2*PIXELS_PER_UNIT;
    
    float dirC1X = c1X;
    float dirC1Y = c1Y;
    float dirC2X = c2X;
    float dirC2Y = c2Y;

    c1X = convertHorizontalPoint(c1X);
    c1Y = convertVerticalPoint(c1Y);
    c2X = convertHorizontalPoint(c2X);
    c2Y = convertVerticalPoint(c2Y);

    m_mouse->inGoal() ? glColor3fv(DARKGREEN) : glColor3fv(m_color);
    glBegin(GL_POLYGON);
        glVertex2f(c1X, c1Y);
        glVertex2f(c1X, c2Y);
        glVertex2f(c2X, c2Y);
        glVertex2f(c2X, c1Y);
    glEnd();

    drawDirection(dirC1X, dirC1Y, dirC2X, dirC2Y);
}

void MouseGraphic::drawDirection(float c1X, float c1Y, float c2X, float c2Y){

    float p1X = 0;
    float p1Y = 0;
    float p2X = 0;
    float p2Y = 0;
    float p3X = 0;
    float p3Y = 0;

    switch(m_mouse->getDirection()){
        case NORTH:
            p1X = (3*c1X + c2X)/4;
            p1Y = (c1Y + c2Y)/2;
            p2X = (c1X + c2X)/2;
            p2Y = c2Y;
            p3X = (c1X + 3*c2X)/4;
            p3Y = (c1Y + c2Y)/2;
            break;
        case EAST:
            p1X = (c1X + c2X)/2;
            p1Y = (3*c1Y + c2Y)/4;
            p2X = (c1X + c2X)/2;
            p2Y = (c1Y + 3*c2Y)/4;
            p3X = c2X;
            p3Y = (c1Y + c2Y)/2;
            break;
        case SOUTH:
            p1X = (3*c1X + c2X)/4;
            p1Y = (c1Y + c2Y)/2;
            p2X = (c1X + c2X)/2;
            p2Y = c1Y;
            p3X = (c1X + 3*c2X)/4;
            p3Y = (c1Y + c2Y)/2;
            break;
        case WEST:
            p1X = (c1X + c2X)/2;
            p1Y = (3*c1Y + c2Y)/4;
            p2X = (c1X + c2X)/2;
            p2Y = (c1Y + 3*c2Y)/4;
            p3X = c1X;
            p3Y = (c1Y + c2Y)/2;
            break;
    }
    
    p1X = convertHorizontalPoint(p1X);
    p1Y = convertVerticalPoint(p1Y);
    p2X = convertHorizontalPoint(p2X);
    p2Y = convertVerticalPoint(p2Y);
    p3X = convertHorizontalPoint(p3X);
    p3Y = convertVerticalPoint(p3Y);

    glColor3fv(GREEN);
    glBegin(GL_POLYGON);
        glVertex2f(p1X, p1Y);
        glVertex2f(p2X, p2Y);
        glVertex2f(p3X, p3Y);
    glEnd();
}

} // namespace sim
