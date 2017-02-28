#include "MouseGraphic.h"

#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

namespace mms {

MouseGraphic::MouseGraphic(const Mouse* mouse) :
    m_mouse(mouse) {
}

Cartesian MouseGraphic::getInitialMouseTranslation() const {
    return m_mouse->getInitialTranslation();
}

QPair<Cartesian, Radians> MouseGraphic::getCurrentMousePosition() const {
    return {
        m_mouse->getCurrentTranslation(),
        m_mouse->getCurrentRotation(),
    };
}

QVector<TriangleGraphic> MouseGraphic::draw(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const {

    QVector<TriangleGraphic> buffer;

    // First, we draw the body
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentBodyPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.value(P()->mouseBodyColor()), 1.0));

    // Next, draw the center of mass
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentCenterOfMassPolygon(currentTranslation, currentRotation),
        STRING_TO_COLOR.value(P()->mouseCenterOfMassColor()), 1.0));

    // Next, we draw the wheels
    for (const Polygon& wheelPolygon :
            m_mouse->getCurrentWheelPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            wheelPolygon,
            STRING_TO_COLOR.value(P()->mouseWheelColor()), 1.0));
    }

    // Next, we draw the wheel speed indicators
    for (const Polygon& wheelSpeedIndicatorPolygon :
            m_mouse->getCurrentWheelSpeedIndicatorPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            wheelSpeedIndicatorPolygon,
            STRING_TO_COLOR.value(P()->mouseWheelSpeedIndicatorColor()), 1.0));
    }

    // Next, we draw the sensors
    for (const Polygon& sensorPolygon :
            m_mouse->getCurrentSensorPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            sensorPolygon,
            STRING_TO_COLOR.value(P()->mouseSensorColor()), 1.0));
    }

    // Lastly, we draw the sensor views
    for (const Polygon& polygon :
            m_mouse->getCurrentSensorViewPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            polygon,
            STRING_TO_COLOR.value(P()->mouseViewColor()), 1.0));
    }

    // Uncomment to draw collision polygon
    /*
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentCollisionPolygon(currentTranslation, currentRotation),
        Color::GRAY, .5);
    */

    return buffer;
}

} // namespace mms
