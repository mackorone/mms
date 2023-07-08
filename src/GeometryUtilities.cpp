#include "GeometryUtilities.h"

namespace mms {

Coordinate GeometryUtilities::translateVertex(const Coordinate &vertex,
                                              const Coordinate &translation) {
  return vertex + translation;
}

Coordinate GeometryUtilities::rotateVertexAroundPoint(const Coordinate &vertex,
                                                      const Coordinate &point,
                                                      const Angle &angle) {
  Coordinate relativeVertex = Coordinate::Cartesian(
      vertex.getX() - point.getX(), vertex.getY() - point.getY());
  Coordinate rotatedRelativeVertex = Coordinate::Polar(
      relativeVertex.getRho(), relativeVertex.getTheta() + angle);
  Coordinate rotatedVertex =
      Coordinate::Cartesian(rotatedRelativeVertex.getX() + point.getX(),
                            rotatedRelativeVertex.getY() + point.getY());
  return rotatedVertex;
}

}  // namespace mms
