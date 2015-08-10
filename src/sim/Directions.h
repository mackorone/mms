#pragma once

#include <map>
#include <vector>

#include "units/Degrees.h"

namespace sim {

enum Direction { NORTH, EAST, SOUTH, WEST };

// This vector serves two functions:
// 1) Serves as a convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in the maze files
static const std::vector<Direction> DIRECTIONS { NORTH, EAST, SOUTH, WEST };

static const std::map<Direction, Degrees> DIRECTION_TO_ANGLE {
    {NORTH, Degrees(0)},
    {EAST, Degrees(270)},
    {SOUTH, Degrees(180)},
    {WEST, Degrees(90)},
};

static const std::map<Direction, char> DIRECTION_TO_CHAR {
    {NORTH, 'n'},
    {EAST, 'e'},
    {SOUTH, 's'},
    {WEST, 'w'},
};

static const std::map<char, Direction> CHAR_TO_DIRECTION {
    {'n', NORTH},
    {'e', EAST},
    {'s', SOUTH},
    {'w', WEST},
};

static const std::map<std::string, Direction> STRING_TO_DIRECTION {
    {"NORTH", NORTH},
    {"EAST", EAST},
    {"SOUTH", SOUTH},
    {"WEST", WEST},
};

} // namespace sim
