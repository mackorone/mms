#include "MouseGraphic.h"

#include "Constants.h"
#include "Param.h"
#include "Mouse.h"
#include "units/Cartesian.h"
#include "Utilities.h"

namespace sim{

MouseGraphic::MouseGraphic(Mouse* mouse) : m_mouse(mouse), m_color(BLUE)
{ }

MouseGraphic::~MouseGraphic()
{ }

void MouseGraphic::draw() {

    /*
    std::vector<Cartesian> vertices;
    for (int i = 0; i < m_mouse->getVertices().getVertices().size(); i += 1) {
        vertices.push_back(m_mouse->getTranslation() + m_mouse->getVertices().getVertices().at(i));
    }
    Polygon p;
    p.setVertices(vertices);
    */
    //m_mouse->inGoal() ? glColor3fv(DARKGREEN) : glColor3fv(m_color);
    /*
        glVertex2f(c1X, c1Y);
        glVertex2f(c1X, c2Y);
        glVertex2f(c2X, c2Y);
        glVertex2f(c2X, c1Y);
    glEnd();
    */
    glColor3fv(BLUE);
    //drawPolygon(physicalToOpenGl(p));

    // TODO...
    drawPolygon(physicalToOpenGl(m_mouse->getShape()
                                .translate(m_mouse->getTranslation())));

    //drawDirection(dirC1X, dirC1Y, dirC2X, dirC2Y);
}

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

} // namespace sim
