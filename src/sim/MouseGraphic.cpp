#include "MouseGraphic.h"

#include <Cartesian.h>

#include "GraphicUtilities.h"
#include "Mouse.h"
#include "Param.h"
#include "State.h"

namespace sim{

MouseGraphic::MouseGraphic(const Mouse* mouse) : m_mouse(mouse) {
}

void MouseGraphic::draw() {

    // Only draw the mouse if it's visible and the interface type has been declared
    // (which also means that the mouse has been initialized)
    if (!S()->mouseVisible() || S()->interfaceType() == UNDECLARED) {
        return;
    }

    // First, we draw the body
    glColor3fv(COLOR_STRINGS.at(P()->mouseBodyColor()));
    GraphicUtilities::drawPolygon(m_mouse->getBodyPolygon());

    // Next, we draw the wheels
    glColor3fv(COLOR_STRINGS.at(P()->mouseWheelColor()));
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        GraphicUtilities::drawPolygon(wheelPolygon);
    }

    // Only draw the sensors if we're using a discrete interface
    if (S()->interfaceType() == CONTINUOUS) {

        // Next, we draw the sensors
        glColor3fv(COLOR_STRINGS.at(P()->mouseWheelColor()));
        for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
            GraphicUtilities::drawPolygon(sensorPolygon);
        }

        // Lastly, we draw the sensor views
        glColor3fv(COLOR_STRINGS.at(P()->mouseSensorColor()));
        for (Polygon polygon : m_mouse->getViewPolygons()) {
            GraphicUtilities::drawPolygon(polygon);
        }
    }
}

} // namespace sim
