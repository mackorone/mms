#pragma once

#include "units/Angle.h"
#include "units/Coordinate.h"

namespace mms {

class GeometryUtilities {
 public:
  GeometryUtilities() = delete;

  static Coordinate translateVertex(const Coordinate &vertex,
                                    const Coordinate &translation);

  static Coordinate rotateVertexAroundPoint(const Coordinate &vertex,
                                            const Coordinate &point,
                                            const Angle &angle);
};

}  // namespace mms
