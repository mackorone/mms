#pragma once

#include <vector>

namespace sim {

enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };
static const std::vector<Direction> DIRECTIONS {NORTH, EAST, SOUTH, WEST};

} // namespace sim
