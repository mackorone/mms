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

    // Draw the wheel polygon
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentWheelPolygon(currentTranslation, currentRotation),
        ColorManager::get()->getMouseWheelColor(), 1.0));

    // Overlay the body polygon
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentBodyPolygon(currentTranslation, currentRotation),
        ColorManager::get()->getMouseBodyColor(), 1.0));

    // Overlay the center of mass polygon
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentCenterOfMassPolygon(currentTranslation, currentRotation),
        ColorManager::get()->getMouseCenterOfMassColor(), 1.0));

    return buffer;
}

} // namespace mms
