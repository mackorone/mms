#include "MouseGraphic.h"

#include "units/Cartesian.h"

#include "GraphicUtilities.h"
#include "Mouse.h"
#include "Param.h"
#include "State.h"

namespace sim{

MouseGraphic::MouseGraphic(const Mouse* mouse) : m_mouse(mouse) {
}

void MouseGraphic::draw(const Coordinate& currentTranslation, const Angle& currentRotation) const {

    // First, we draw the body
    GraphicUtilities::drawMousePolygon(
        m_mouse->getCurrentBodyPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.at(P()->mouseBodyColor()), 1.0);

    // Next, draw the center of mass
    GraphicUtilities::drawMousePolygon(
        m_mouse->getCurrentCenterOfMassPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.at(P()->mouseCenterOfMassColor()), 1.0);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getCurrentWheelPolygons(currentTranslation, currentRotation)) {
        GraphicUtilities::drawMousePolygon(
            wheelPolygon,
            STRING_TO_COLOR.at(P()->mouseWheelColor()), 1.0);
    }

    // Next, we draw the wheel speed indicators
    for (Polygon wheelSpeedIndicatorPolygon : m_mouse->getCurrentWheelSpeedIndicatorPolygons(currentTranslation, currentRotation)) {
        GraphicUtilities::drawMousePolygon(
            wheelSpeedIndicatorPolygon,
            STRING_TO_COLOR.at(P()->mouseWheelSpeedIndicatorColor()), 1.0);
    }

    // Next, we draw the sensors
    for (Polygon sensorPolygon : m_mouse->getCurrentSensorPolygons(currentTranslation, currentRotation)) {
        GraphicUtilities::drawMousePolygon(
            sensorPolygon,
            STRING_TO_COLOR.at(P()->mouseSensorColor()), 1.0);
    }

    // Lastly, we draw the sensor views
    for (Polygon polygon : m_mouse->getCurrentSensorViewPolygons(currentTranslation, currentRotation)) {
        GraphicUtilities::drawMousePolygon(
            polygon,
            STRING_TO_COLOR.at(P()->mouseViewColor()), 1.0);
    }
}

} // namespace sim
