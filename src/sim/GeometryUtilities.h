#pragma once

#include <utility>

#include <Cartesian.h>
#include <MetersSquared.h>

#include "Polygon.h"

namespace sim {

// Returns true if the line segments A and B intersect
bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A, 
                    const std::pair<const Cartesian&, const Cartesian&>& B);

// Returns the area of a polygon
MetersSquared polygonArea(const Polygon& polygon);

} // namespace sim
