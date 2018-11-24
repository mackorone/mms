#include "Direction.h"

#include "ContainerUtilities.h"

namespace mms {

const QVector<Direction>& DIRECTIONS() {
    static const QVector<Direction> vector = {
        Direction::NORTH,
        Direction::EAST,
        Direction::SOUTH,
        Direction::WEST,
    };
    return vector;
}

const QMap<Direction, Direction>& DIRECTION_ROTATE_LEFT() {
    static const QMap<Direction, Direction> map = {
        {Direction::NORTH, Direction::WEST},
        {Direction::EAST, Direction::NORTH},
        {Direction::SOUTH, Direction::EAST},
        {Direction::WEST, Direction::SOUTH},
    };
    return map;
}

const QMap<Direction, Direction>& DIRECTION_ROTATE_RIGHT() {
    static const QMap<Direction, Direction> map = {
        {Direction::NORTH, Direction::EAST},
        {Direction::EAST, Direction::SOUTH},
        {Direction::SOUTH, Direction::WEST},
        {Direction::WEST, Direction::NORTH},
    };
    return map;
}

const QMap<Direction, Direction>& DIRECTION_OPPOSITE() {
    static const QMap<Direction, Direction> map = {
        {Direction::NORTH, Direction::SOUTH},
        {Direction::EAST, Direction::WEST},
        {Direction::SOUTH, Direction::NORTH},
        {Direction::WEST, Direction::EAST},
    };
    return map;
}

const QMap<Direction, Angle>& DIRECTION_TO_ANGLE() {
    static const QMap<Direction, Angle> map = {
        {Direction::NORTH, Angle::Degrees(90)},
        {Direction::EAST, Angle::Degrees(0)},
        {Direction::SOUTH, Angle::Degrees(270)},
        {Direction::WEST, Angle::Degrees(180)},
    };
    return map;
}

const QMap<Direction, QString>& DIRECTION_TO_STRING() {
    static const QMap<Direction, QString> map = {
        {Direction::NORTH, "NORTH"},
        {Direction::EAST, "EAST"},
        {Direction::SOUTH, "SOUTH"},
        {Direction::WEST, "WEST"},
    };
    return map;
}

const QMap<QString, Direction>& STRING_TO_DIRECTION() {
    static const QMap<QString, Direction> map =
        ContainerUtilities::inverse(DIRECTION_TO_STRING());
    return map;
}

const QMap<Direction, QChar>& DIRECTION_TO_CHAR() {
    static const QMap<Direction, QChar> map = {
        {Direction::NORTH, 'n'},
        {Direction::EAST, 'e'},
        {Direction::SOUTH, 's'},
        {Direction::WEST, 'w'},
    };
    return map;
};

const QMap<QChar, Direction>& CHAR_TO_DIRECTION() {
    static const QMap<QChar, Direction> map =
        ContainerUtilities::inverse(DIRECTION_TO_CHAR());
    return map;
}

} 
