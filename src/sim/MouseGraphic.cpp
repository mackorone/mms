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

    // Only draw the mouse if it's visible
    // TODO: Does this need to be a feature???
    float alpha = S()->mouseVisible() ? 1 : 0;

    // First, we draw the body
    GraphicUtilities::drawMousePolygon(m_mouse->getBodyPolygon(), COLOR_STRINGS.at(P()->mouseBodyColor()), alpha);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        GraphicUtilities::drawMousePolygon(wheelPolygon, COLOR_STRINGS.at(P()->mouseWheelColor()), alpha);
    }

    // Only draw the sensors if we're using a discrete interface
    if (S()->interfaceType() == CONTINUOUS) {

        // Next, we draw the sensors
        for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
            // TODO: Specific color for this
            GraphicUtilities::drawMousePolygon(sensorPolygon, COLOR_STRINGS.at(P()->mouseWheelColor()), alpha);
        }

        // Lastly, we draw the sensor views
        for (Polygon polygon : m_mouse->getViewPolygons()) {
            // TODO: rename to sensor view color
            GraphicUtilities::drawMousePolygon(polygon, COLOR_STRINGS.at(P()->mouseSensorColor()), alpha);
        }
    }
}

} // namespace sim
