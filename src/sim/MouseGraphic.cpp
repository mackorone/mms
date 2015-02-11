#include "MouseGraphic.h"

#include "Constants.h"
#include "Param.h"
#include "Mouse.h"
#include "units/Cartesian.h"
#include "Utilities.h"

namespace sim{

MouseGraphic::MouseGraphic(Mouse* mouse) : m_mouse(mouse), m_color(BLUE) {
}

void MouseGraphic::draw() {

    glColor3fv(m_color);

    for (int i = m_mouse->getShapes().size() - 1; i >= 0 ; i -= 1) {
        if (i > 0) {
            glColor3fv(GREEN);
        }
        else {
            glColor3fv(m_color);
        }
        Polygon p = m_mouse->getShapes().at(i);
        drawPolygon(physicalToOpenGl(p.rotate(m_mouse->getRotation()).translate(m_mouse->getTranslation())));
        // TODO: Do the rotation
    }

    //drawDirection(dirC1X, dirC1Y, dirC2X, dirC2Y);
}

#if(0)
void MouseGraphic::drawDirection(float c1X, float c1Y, float c2X, float c2Y) {

    // TODO

    /*
    float p1X = 0;
    float p1Y = 0;
    float p2X = 0;
    float p2Y = 0;
    float p3X = 0;
    float p3Y = 0;

    switch(m_mouse->getDirection()) {
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
    */
}
#endif

} // namespace sim
