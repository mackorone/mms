#include "Direction.h"

namespace mms {

const QVector<Direction>& DIRECTIONS() {
    static const QVector<Direction> vector = {
        Direction::NORTH,
        Direction::EAST,
        Direction::SOUTH,
        Direction::WEST,
        Direction::NORTHEAST,
        Direction::NORTHWEST,
        Direction::SOUTHEAST,
        Direction::SOUTHWEST,
        Direction::EASTNORTH,
        Direction::EASTSOUTH,
        Direction::WESTNORTH,
        Direction::WESTSOUTH,
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

const QMap<QChar, Direction>& CHAR_TO_DIRECTION() {
    static const QMap<QChar, Direction> map = {
        {'n', Direction::NORTH},
        {'e', Direction::EAST},
        {'s', Direction::SOUTH},
        {'w', Direction::WEST},
    };
    return map;
}

const QMap<Direction, Angle>& DIRECTION_TO_ANGLE() {
    static const QMap<Direction, Angle> map = {
        {Direction::NORTH, Angle::Degrees(90)},
        {Direction::EAST, Angle::Degrees(0)},
        {Direction::SOUTH, Angle::Degrees(270)},
        {Direction::WEST, Angle::Degrees(180)},
        {Direction::NORTHEAST, Angle::Degrees(45)},
        {Direction::NORTHWEST, Angle::Degrees(135)},
        {Direction::SOUTHEAST, Angle::Degrees(315)},
        {Direction::SOUTHWEST, Angle::Degrees(225)},
        {Direction::EASTNORTH, Angle::Degrees(45)},
        {Direction::EASTSOUTH, Angle::Degrees(315)},
        {Direction::WESTNORTH, Angle::Degrees(135)},
        {Direction::WESTSOUTH, Angle::Degrees(225)},
    };
    return map;
}

} 
