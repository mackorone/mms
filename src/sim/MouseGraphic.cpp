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

    // Only draw the mouse if its interface type has been declared
    // (which also means that the mouse has been initialized)
    if (S()->interfaceType() == UNDECLARED) {
        return;
    }

    // First, we draw the body
    GraphicUtilities::drawMousePolygon(m_mouse->getBodyPolygon(), COLOR_STRINGS.at(P()->mouseBodyColor()));

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        GraphicUtilities::drawMousePolygon(wheelPolygon, COLOR_STRINGS.at(P()->mouseWheelColor()));
    }

    // Only draw the sensors if we're using a discrete interface
    if (S()->interfaceType() == CONTINUOUS) {

        // Next, we draw the sensors
        for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
            GraphicUtilities::drawMousePolygon(sensorPolygon, COLOR_STRINGS.at(P()->mouseSensorColor()));
        }

        // Lastly, we draw the sensor views
        for (Polygon polygon : m_mouse->getViewPolygons()) {
            GraphicUtilities::drawMousePolygon(polygon, COLOR_STRINGS.at(P()->mouseViewColor()));
        }
    }
}

} // namespace sim
