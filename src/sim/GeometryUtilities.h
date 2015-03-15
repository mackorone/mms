#pragma once

#include <utility>

#include <Cartesian.h>
#include <MetersSquared.h>

#include "Polygon.h"

namespace sim {

// Returns true if the line segments A and B intersect
bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A, 
                    const std::pair<const Cartesian&, const Cartesian&>& B);

// Finds the point at which the line segments A and B intersect
// Note: This should only be called if the lines actually intersect
Cartesian getIntersectionPoint(const std::pair<const Cartesian&, const Cartesian&>& A, 
                               const std::pair<const Cartesian&, const Cartesian&>& B);

// Returns the area of a polygon
MetersSquared polygonArea(const Polygon& polygon);

// Retrieves every pair of neighboring line segments in a polygon
std::vector<std::pair<Cartesian, Cartesian>> getLineSegments(const Polygon& polygon);

} // namespace sim
