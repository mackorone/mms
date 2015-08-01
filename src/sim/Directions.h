#pragma once

#include <map>
#include <vector>

namespace sim {

enum Direction { NORTH, EAST, SOUTH, WEST };

// This vector serves two functions:
// 1) Serves as a convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in the maze files
static const std::vector<Direction> DIRECTIONS { NORTH, EAST, SOUTH, WEST };

static const std::map<std::string, Direction> DIRECTION_STRINGS {
    {"NORTH", NORTH},
    {"EAST", EAST},
    {"SOUTH", SOUTH},
    {"WEST", WEST},
};

} // namespace sim
