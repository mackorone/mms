#pragma once

#include <vector>

namespace sim {

// Create the Direction type
enum Direction { NORTH, EAST, SOUTH, WEST };

// This vector serves two functions:
// 1) Serves as a convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in the maze files
static const std::vector<Direction> DIRECTIONS { NORTH, EAST, SOUTH, WEST };

} // namespace sim
