#include "SimUtilities.h"

#include <QDateTime>

#include "AssertMacros.h"

namespace mms {

double SimUtilities::getHighResTimestamp() {
  return QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
}

QVector<TriangleGraphic> SimUtilities::polygonToTriangleGraphics(
    const Polygon &polygon, Color color, unsigned char alpha) {
  QVector<Triangle> triangles = polygon.getTriangles();
  QVector<TriangleGraphic> triangleGraphics;
  RGB colorValues = COLOR_TO_RGB().value(color);
  for (Triangle triangle : triangles) {
    TriangleGraphic graphic;
    graphic.p1 = {
        static_cast<float>(triangle.p1.getX().getMeters()),
        static_cast<float>(triangle.p1.getY().getMeters()),
        colorValues,
        alpha,
    };
    graphic.p2 = {
        static_cast<float>(triangle.p2.getX().getMeters()),
        static_cast<float>(triangle.p2.getY().getMeters()),
        colorValues,
        alpha,
    };
    graphic.p3 = {
        static_cast<float>(triangle.p3.getX().getMeters()),
        static_cast<float>(triangle.p3.getY().getMeters()),
        colorValues,
        alpha,
    };
    triangleGraphics.append(graphic);
  }
  return triangleGraphics;
}

}  // namespace mms
