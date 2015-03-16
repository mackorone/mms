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
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        drawPolygon(wheelPolygon);
    }

    // Next, we draw the body
    glColor3fv(COLORS.at(P()->mouseBodyColor()));
    drawPolygon(m_mouse->getBodyPolygon());

    // Next, we draw the sensors
    glColor3fv(COLORS.at(P()->mouseWheelColor()));
    for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
        drawPolygon(sensorPolygon);
    }

    // Lastly, we draw the sensor views
    glColor3fv(WHITE);
    for (Polygon polygon : m_mouse->getViewPolygons()) {
        drawPolygon(polygon);
    }
}

} // namespace sim
