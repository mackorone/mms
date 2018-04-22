#include "MouseGraphic.h"

#include "ColorManager.h"
#include "Param.h"
#include "SimUtilities.h"

namespace mms {

MouseGraphic::MouseGraphic(const Mouse* mouse) :
    m_mouse(mouse) {
}

Coordinate MouseGraphic::getInitialMouseTranslation() const {
    return m_mouse->getInitialTranslation();
}

QPair<Coordinate, Angle> MouseGraphic::getCurrentMousePosition() const {
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
        ColorManager::get()->getMouseBodyColor(), 1.0));

    // Next, draw the center of mass
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentCenterOfMassPolygon(currentTranslation, currentRotation),
        ColorManager::get()->getMouseCenterOfMassColor(), 1.0));

    // Next, we draw the wheels
    for (const Polygon& wheelPolygon :
            m_mouse->getCurrentWheelPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            wheelPolygon,
            ColorManager::get()->getMouseWheelColor(), 1.0));
    }

    // Next, we draw the sensors
    for (const Polygon& sensorPolygon :
            m_mouse->getCurrentSensorPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            sensorPolygon,
            ColorManager::get()->getMouseSensorColor(), 1.0));
    }

    // Lastly, we draw the sensor views
    for (const Polygon& polygon :
            m_mouse->getCurrentSensorViewPolygons(currentTranslation, currentRotation)) {
        buffer.append(SimUtilities::polygonToTriangleGraphics(
            polygon,
            ColorManager::get()->getMouseVisionColor(), 1.0));
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
