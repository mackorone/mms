#include "MouseGraphic.h"

#include "units/Cartesian.h"

#include "GraphicUtilities.h"
#include "Mouse.h"
#include "Param.h"
#include "State.h"

namespace sim{

MouseGraphic::MouseGraphic(const Mouse* mouse) : m_mouse(mouse) {
}

void MouseGraphic::draw() {

    // Only draw the mouse if it has been initialized
    // TODO: MACK - get rid of this once we factor out getInitialized
    if (!m_mouse->getInitialized()) {
        return;
    }

    // First, we draw the body
    GraphicUtilities::drawMousePolygon(m_mouse->getBodyPolygon(), STRING_TO_COLOR.at(P()->mouseBodyColor()), 1.0);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        GraphicUtilities::drawMousePolygon(wheelPolygon, STRING_TO_COLOR.at(P()->mouseWheelColor()), 1.0);
    }

    // Only draw the sensors and views if we're using a continuous interface
    float sensorAlpha = (S()->interfaceType() == InterfaceType::CONTINUOUS ? 1.0 : 0.0);

    // Next, we draw the sensors
    for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
        GraphicUtilities::drawMousePolygon(sensorPolygon, STRING_TO_COLOR.at(P()->mouseSensorColor()), sensorAlpha);
    }

    // Lastly, we draw the sensor views
    for (Polygon polygon : m_mouse->getViewPolygons()) {
        GraphicUtilities::drawMousePolygon(polygon, STRING_TO_COLOR.at(P()->mouseViewColor()), sensorAlpha);
    }
}

} // namespace sim
