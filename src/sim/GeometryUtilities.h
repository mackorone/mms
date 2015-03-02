#pragma once

#include <utility>

#include <Cartesian.h>

namespace sim {

bool linesIntersect(std::pair<const Cartesian&, const Cartesian&> A,
                    std::pair<const Cartesian&, const Cartesian&> B);

} // namespace sim
