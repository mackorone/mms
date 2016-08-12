#pragma once

#include <QMap>
#include <QString>
#include <QVector>

#include "ContainerUtilities.h"
#include "units/Degrees.h"

namespace mms {

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

// This vector serves a few functions:
// 1) A convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in .NUM maze files
// 3) Determines the location of wall information in the vertex buffer
static const QVector<Direction> DIRECTIONS {
    Direction::NORTH,
    Direction::EAST,
    Direction::SOUTH,
    Direction::WEST,
};

static const QMap<Direction, Direction> DIRECTION_ROTATE_LEFT {
    {Direction::NORTH, Direction::WEST},
    {Direction::EAST, Direction::NORTH},
    {Direction::SOUTH, Direction::EAST},
    {Direction::WEST, Direction::SOUTH},
};

static const QMap<Direction, Direction> DIRECTION_ROTATE_RIGHT {
    {Direction::NORTH, Direction::EAST},
    {Direction::EAST, Direction::SOUTH},
    {Direction::SOUTH, Direction::WEST},
    {Direction::WEST, Direction::NORTH},
};

static const QMap<Direction, Direction> DIRECTION_OPPOSITE {
    {Direction::NORTH, Direction::SOUTH},
    {Direction::EAST, Direction::WEST},
    {Direction::SOUTH, Direction::NORTH},
    {Direction::WEST, Direction::EAST},
};

static const QMap<Direction, Degrees> DIRECTION_TO_ANGLE {
    {Direction::NORTH, Degrees(90)},
    {Direction::EAST, Degrees(0)},
    {Direction::SOUTH, Degrees(270)},
    {Direction::WEST, Degrees(180)},
};

static const QMap<Direction, QString> DIRECTION_TO_STRING {
    {Direction::NORTH, "NORTH"},
    {Direction::EAST, "EAST"},
    {Direction::SOUTH, "SOUTH"},
    {Direction::WEST, "WEST"},
};

static const QMap<QString, Direction> STRING_TO_DIRECTION =
    ContainerUtilities::inverse(DIRECTION_TO_STRING);

static const QMap<Direction, QChar> DIRECTION_TO_CHAR {
    {Direction::NORTH, 'n'},
    {Direction::EAST, 'e'},
    {Direction::SOUTH, 's'},
    {Direction::WEST, 'w'},
};

static const QMap<QChar, Direction> CHAR_TO_DIRECTION =
    ContainerUtilities::inverse(DIRECTION_TO_CHAR);

} // namespace mms
