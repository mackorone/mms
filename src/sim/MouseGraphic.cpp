#include "MouseGraphic.h"

#include "units/Cartesian.h"

#include "Mouse.h"
#include "Param.h"
#include "State.h"

#include "View.h" // TODO: MACK

namespace sim{

MouseGraphic::MouseGraphic(const Mouse* mouse, View* view) : m_mouse(mouse), m_view(view) {
}

void MouseGraphic::draw(const Coordinate& currentTranslation, const Angle& currentRotation) const {

    // First, we draw the body
    m_view->drawMousePolygon(
        m_mouse->getCurrentBodyPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.at(P()->mouseBodyColor()), 1.0);

    // Next, draw the center of mass
    m_view->drawMousePolygon(
        m_mouse->getCurrentCenterOfMassPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.at(P()->mouseCenterOfMassColor()), 1.0);

    // Next, we draw the wheels
    for (Polygon wheelPolygon : m_mouse->getCurrentWheelPolygons(currentTranslation, currentRotation)) {
        m_view->drawMousePolygon(
            wheelPolygon,
            STRING_TO_COLOR.at(P()->mouseWheelColor()), 1.0);
    }

    // Next, we draw the wheel speed indicators
    for (Polygon wheelSpeedIndicatorPolygon : m_mouse->getCurrentWheelSpeedIndicatorPolygons(currentTranslation, currentRotation)) {
        m_view->drawMousePolygon(
            wheelSpeedIndicatorPolygon,
            STRING_TO_COLOR.at(P()->mouseWheelSpeedIndicatorColor()), 1.0);
    }

    // Next, we draw the sensors
    for (Polygon sensorPolygon : m_mouse->getCurrentSensorPolygons(currentTranslation, currentRotation)) {
        m_view->drawMousePolygon(
            sensorPolygon,
            STRING_TO_COLOR.at(P()->mouseSensorColor()), 1.0);
    }

    // Lastly, we draw the sensor views
    for (Polygon polygon : m_mouse->getCurrentSensorViewPolygons(currentTranslation, currentRotation)) {
        m_view->drawMousePolygon(
            polygon,
            STRING_TO_COLOR.at(P()->mouseViewColor()), 1.0);
    }
}

} // namespace sim
