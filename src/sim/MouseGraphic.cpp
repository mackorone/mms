#include "MouseGraphic.h"

#include <Cartesian.h>

#include "Param.h"
#include "Mouse.h"
#include "GraphicUtilities.h"

namespace sim{

MouseGraphic::MouseGraphic(const Mouse* mouse) : m_mouse(mouse) {
}

void MouseGraphic::draw() {

    // First, we draw the wheels
    glColor3fv(COLORS.at(P()->mouseWheelColor()));
    for (int i = 1; i < m_mouse->getBodyPolygons().size(); i += 1) {
        drawPolygon(m_mouse->getBodyPolygons().at(i));
    }

    // Next, we draw the body
    glColor3fv(COLORS.at(P()->mouseBodyColor()));
    drawPolygon(m_mouse->getBodyPolygons().at(0));

    // Lastly, we draw the sensors
    glColor3fv(WHITE);
    for (Polygon polygon : m_mouse->getSensorPolygons()) {
        drawPolygon(polygon);
    }
}

} // namespace sim
