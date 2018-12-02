#include "MouseGraphic.h"

#include "ColorManager.h"
#include "SimUtilities.h"

namespace mms {

MouseGraphic::MouseGraphic(const Mouse* mouse) :
    m_mouse(mouse) {
}

QVector<TriangleGraphic> MouseGraphic::draw() const {

    QVector<TriangleGraphic> buffer;
    Coordinate currentTranslation = m_mouse->getCurrentTranslation();
    Angle currentRotation = m_mouse->getCurrentRotation();

    // Draw the wheel polygon
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentWheelPolygon(currentTranslation, currentRotation),
        ColorManager::getMouseWheelColor(), 255));

    // Overlay the body polygon
    buffer.append(SimUtilities::polygonToTriangleGraphics(
        m_mouse->getCurrentBodyPolygon(currentTranslation, currentRotation),
        ColorManager::getMouseBodyColor(), 255));

    return buffer;
}

} 
