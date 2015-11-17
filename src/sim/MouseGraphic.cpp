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

    // TODO: MACK
    Cartesian translation = m_mouse->getCurrentTranslation();
    Radians rotation = m_mouse->getCurrentRotation();

    // First, we draw the body
    GraphicUtilities::drawMousePolygon(
        m_mouse->getBodyPolygon(translation, rotation), STRING_TO_COLOR.at(P()->mouseBodyColor()), 1.0);

    // Next, draw the center of mass
    GraphicUtilities::drawMousePolygon(
        m_mouse->getCenterOfMassPolygon(translation, rotation), STRING_TO_COLOR.at(P()->mouseCenterOfMassColor()), 1.0);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getWheelPolygons(translation, rotation)) {
        GraphicUtilities::drawMousePolygon(
            wheelPolygon, STRING_TO_COLOR.at(P()->mouseWheelColor()), 1.0);
    }

    // Next, we draw the wheel speed indicators
    for (Polygon wheelSpeedIndicatorPolygon : m_mouse->getWheelSpeedIndicatorPolygons(translation, rotation)) {
        GraphicUtilities::drawMousePolygon(
            wheelSpeedIndicatorPolygon, STRING_TO_COLOR.at(P()->mouseWheelSpeedIndicatorColor()), 1.0);
    }

    // Only draw the sensors and views if we're using a continuous interface
    float sensorAlpha = (S()->interfaceType() == InterfaceType::CONTINUOUS ? 1.0 : 0.0);

    // Next, we draw the sensors
    for (Polygon sensorPolygon : m_mouse->getSensorPolygons(translation, rotation)) {
        GraphicUtilities::drawMousePolygon(
            sensorPolygon, STRING_TO_COLOR.at(P()->mouseSensorColor()), sensorAlpha);
    }

    // Lastly, we draw the sensor views
    for (Polygon polygon : m_mouse->getViewPolygons(translation, rotation)) {
        GraphicUtilities::drawMousePolygon(
            polygon, STRING_TO_COLOR.at(P()->mouseViewColor()), sensorAlpha);
    }
}

} // namespace sim
