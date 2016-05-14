#pragma once

#include <map>
#include <vector>

#include "ContainerUtilities.h"
#include "units/Degrees.h"

namespace sim {

enum class Direction { NORTH, EAST, SOUTH, WEST };

// This vector serves two functions:
// 1) Serves as a convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in the maze files
static const std::vector<Direction> DIRECTIONS {
    Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST,
};

static const std::map<Direction, Direction> DIRECTION_ROTATE_LEFT {
    {Direction::NORTH, Direction::WEST},
    {Direction::EAST, Direction::NORTH},
    {Direction::SOUTH, Direction::EAST},
    {Direction::WEST, Direction::SOUTH},
};

static const std::map<Direction, Direction> DIRECTION_ROTATE_RIGHT {
    {Direction::NORTH, Direction::EAST},
    {Direction::EAST, Direction::SOUTH},
    {Direction::SOUTH, Direction::WEST},
    {Direction::WEST, Direction::NORTH},
};

static const std::map<Direction, Degrees> DIRECTION_TO_ANGLE {
    {Direction::NORTH, Degrees(90)},
    {Direction::EAST, Degrees(0)},
    {Direction::SOUTH, Degrees(270)},
    {Direction::WEST, Degrees(180)},
};

static const std::map<Direction, std::string> DIRECTION_TO_STRING {
    {Direction::NORTH, "NORTH"},
    {Direction::EAST, "EAST"},
    {Direction::SOUTH, "SOUTH"},
    {Direction::WEST, "WEST"},
};

static const std::map<std::string, Direction> STRING_TO_DIRECTION =
    ContainerUtilities::inverse(DIRECTION_TO_STRING);

static const std::map<Direction, char> DIRECTION_TO_CHAR {
    {Direction::NORTH, 'n'},
    {Direction::EAST, 'e'},
    {Direction::SOUTH, 's'},
    {Direction::WEST, 'w'},
};

static const std::map<char, Direction> CHAR_TO_DIRECTION =
    ContainerUtilities::inverse(DIRECTION_TO_CHAR);

} // namespace sim
