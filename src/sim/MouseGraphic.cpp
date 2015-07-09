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
    GraphicUtilities::drawMousePolygon(m_mouse->getBodyPolygon(), COLOR_STRINGS.at(P()->mouseBodyColor()), 1.0);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        GraphicUtilities::drawMousePolygon(wheelPolygon, COLOR_STRINGS.at(P()->mouseWheelColor()), 1.0);
    }

    // Only draw the sensors and views if we're using a continuous interface
    float sensorAlpha = (S()->interfaceType() == CONTINUOUS ? 1.0 : 0.0);

    // Next, we draw the sensors
    for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
        GraphicUtilities::drawMousePolygon(sensorPolygon, COLOR_STRINGS.at(P()->mouseSensorColor()), sensorAlpha);
    }

    // Lastly, we draw the sensor views
    for (Polygon polygon : m_mouse->getViewPolygons()) {
        GraphicUtilities::drawMousePolygon(polygon, COLOR_STRINGS.at(P()->mouseViewColor()), sensorAlpha);
    }
}

} // namespace sim
