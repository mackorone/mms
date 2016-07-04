#include "MouseGraphic.h"

#include "units/Cartesian.h"

#include "Mouse.h"
#include "Param.h"
#include "State.h"

#include "Logging.h"

namespace sim{

MouseGraphic::MouseGraphic(const Mouse* mouse, BufferInterface* bufferInterface) :
        m_mouse(mouse), m_bufferInterface(bufferInterface) {
}

void MouseGraphic::draw(const Coordinate& currentTranslation, const Angle& currentRotation) const {

    // First, we draw the body
    m_bufferInterface->drawMousePolygon(
        m_mouse->getCurrentBodyPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.value(P()->mouseBodyColor().c_str()), 1.0);

    // Next, draw the center of mass
    m_bufferInterface->drawMousePolygon(
        m_mouse->getCurrentCenterOfMassPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.value(P()->mouseCenterOfMassColor().c_str()), 1.0);

    // Next, we draw the wheels
    for (const Polygon& wheelPolygon :
            m_mouse->getCurrentWheelPolygons(currentTranslation, currentRotation)) {
        m_bufferInterface->drawMousePolygon(
            wheelPolygon,
            STRING_TO_COLOR.value(P()->mouseWheelColor().c_str()), 1.0);
    }

    // Next, we draw the wheel speed indicators
    for (const Polygon& wheelSpeedIndicatorPolygon :
            m_mouse->getCurrentWheelSpeedIndicatorPolygons(currentTranslation, currentRotation)) {
        m_bufferInterface->drawMousePolygon(
            wheelSpeedIndicatorPolygon,
            STRING_TO_COLOR.value(P()->mouseWheelSpeedIndicatorColor().c_str()), 1.0);
    }

    // Next, we draw the sensors
    for (const Polygon& sensorPolygon :
            m_mouse->getCurrentSensorPolygons(currentTranslation, currentRotation)) {
        m_bufferInterface->drawMousePolygon(
            sensorPolygon,
            STRING_TO_COLOR.value(P()->mouseSensorColor().c_str()), 1.0);
    }

    // Lastly, we draw the sensor views
    for (const Polygon& polygon :
            m_mouse->getCurrentSensorViewPolygons(currentTranslation, currentRotation)) {
        m_bufferInterface->drawMousePolygon(
            polygon,
            STRING_TO_COLOR.value(P()->mouseViewColor().c_str()), 1.0);
    }

    // Uncomment to draw collision polygon
    /*
    m_bufferInterface->drawMousePolygon(
        m_mouse->getCurrentCollisionPolygon(currentTranslation, currentRotation),
        Color::GRAY, .5);
    */
}

} // namespace sim
