#include "MouseGraphic.h"

#include "ColorManager.h"
#include "SimUtilities.h"

namespace mms {

MouseGraphic::MouseGraphic(const Mouse *mouse) : m_mouse(mouse) {}

QVector<TriangleGraphic> MouseGraphic::draw() const {
  QVector<TriangleGraphic> buffer;
  buffer.append(SimUtilities::polygonToTriangleGraphics(
      m_mouse->getCurrentWheelPolygon(),
      ColorManager::get()->getMouseWheelColor(), 255));
  buffer.append(SimUtilities::polygonToTriangleGraphics(
      m_mouse->getCurrentBodyPolygon(),
      ColorManager::get()->getMouseBodyColor(), 255));
  return buffer;
}

}  // namespace mms
