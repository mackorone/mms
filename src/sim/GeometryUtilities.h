#pragma once

#include <utility>

#include <Cartesian.h>

namespace sim {

// Returns true if the line segments A and B intersect
bool linesIntersect(const std::pair<const Cartesian&, const Cartesian&>& A, 
                    const std::pair<const Cartesian&, const Cartesian&>& B);

} // namespace sim
