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

    // Erase the contents of the TGB so that the mouse can append
    //TBG.erase(TGB.begin() + GraphicUtilities::getMouseStartingIndex(), TGB.end()); // TODO

    // Only draw the mouse if it's visible and the interface type has been declared
    // (which also means that the mouse has been initialized)
    if (!S()->mouseVisible() || S()->interfaceType() == UNDECLARED) {
        return;
    }

    // First, we draw the body
    GraphicUtilities::drawPolygon(m_mouse->getBodyPolygon(), COLOR_STRINGS.at(P()->mouseBodyColor()), 1.0);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons()) {
        GraphicUtilities::drawPolygon(wheelPolygon, COLOR_STRINGS.at(P()->mouseWheelColor()), 1.0);
    }

    // Only draw the sensors if we're using a discrete interface
    if (S()->interfaceType() == CONTINUOUS) {

        // Next, we draw the sensors
        for (Polygon sensorPolygon : m_mouse->getSensorPolygons()) {
            GraphicUtilities::drawPolygon(sensorPolygon, COLOR_STRINGS.at(P()->mouseWheelColor()), 1.0); // TODO: Color for this
        }

        // Lastly, we draw the sensor views
        for (Polygon polygon : m_mouse->getViewPolygons()) {
            GraphicUtilities::drawPolygon(polygon, COLOR_STRINGS.at(P()->mouseSensorColor()), 1.0); // TODO: rename to sensor view color
        }
    }
}

} // namespace sim
