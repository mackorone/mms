#pragma once

#include <map>
#include <vector>

#include "units/Degrees.h"

namespace sim {

enum class Direction { NORTH, EAST, SOUTH, WEST };

// This vector serves two functions:
// 1) Serves as a convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in the maze files
static const std::vector<Direction> DIRECTIONS {
    Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST,
};

static const std::map<Direction, Degrees> DIRECTION_TO_ANGLE {
    {Direction::NORTH, Degrees(90)},
    {Direction::EAST, Degrees(0)},
    {Direction::SOUTH, Degrees(270)},
    {Direction::WEST, Degrees(180)},
};

static const std::map<Direction, char> DIRECTION_TO_CHAR {
    {Direction::NORTH, 'n'},
    {Direction::EAST, 'e'},
    {Direction::SOUTH, 's'},
    {Direction::WEST, 'w'},
};

static const std::map<char, Direction> CHAR_TO_DIRECTION {
    {'n', Direction::NORTH},
    {'e', Direction::EAST},
    {'s', Direction::SOUTH},
    {'w', Direction::WEST},
};

static const std::map<Direction, std::string> DIRECTION_TO_STRING {
    {Direction::NORTH, "NORTH"},
    {Direction::EAST, "EAST"},
    {Direction::SOUTH, "SOUTH"},
    {Direction::WEST, "WEST"},
};

static const std::map<std::string, Direction> STRING_TO_DIRECTION {
    {"NORTH", Direction::NORTH},
    {"EAST", Direction::EAST},
    {"SOUTH", Direction::SOUTH},
    {"WEST", Direction::WEST},
};

} // namespace sim
