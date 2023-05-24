#include "Direction.h"

namespace mms {

const QVector<Direction>& DIRECTIONS() {
    static const QVector<Direction> vector = {
        Direction::NORTH,
        Direction::EAST,
        Direction::SOUTH,
        Direction::WEST,
        SemiDirection::NORTHEAST,
        SemiDirection::NORTHWEST,
        SemiDirection::SOUTHEAST,
        SemiDirection::SOUTHWEST,
        SemiDirection::EASTNORTH,
        SemiDirection::EASTSOUTH,
        SemiDirection::WESTNORTH,
        SemiDirection::WESTSOUTH,
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
        {SemiDirection::NORTHEAST, Angle::Degrees(45)},
        {SemiDirection::NORTHWEST, Angle::Degrees(135)},
        {SemiDirection::SOUTHEAST, Angle::Degrees(315)},
        {SemiDirection::SOUTHWEST, Angle::Degrees(225)},
        {SemiDirection::EASTNORTH, Angle::Degrees(45)},
        {SemiDirection::EASTSOUTH, Angle::Degrees(315)},
        {SemiDirection::WESTNORTH, Angle::Degrees(135)},
        {SemiDirection::WESTSOUTH, Angle::Degrees(225)},
    };
    return map;
}

} 
