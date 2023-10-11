#include "Direction.h"

namespace mms {

const QVector<Direction> &CARDINAL_DIRECTIONS() {
  static const QVector<Direction> vector = {
      Direction::NORTH,
      Direction::EAST,
      Direction::SOUTH,
      Direction::WEST,
  };
  return vector;
}

const QVector<SemiDirection> &ORDINAL_DIRECTIONS() {
  static const QVector<SemiDirection> vector = {
      SemiDirection::NORTHEAST,
      SemiDirection::NORTHWEST,
      SemiDirection::SOUTHEAST,
      SemiDirection::SOUTHWEST,
  };
  return vector;
}

const QMap<SemiDirection, Direction> &SEMI_TO_CARDINAL() {
  static const QMap<SemiDirection, Direction> map = {
      {SemiDirection::NORTH, Direction::NORTH},
      {SemiDirection::SOUTH, Direction::SOUTH},
      {SemiDirection::EAST, Direction::EAST},
      {SemiDirection::WEST, Direction::WEST},
  };
  return map;
}

const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_45_LEFT() {
  static const QMap<SemiDirection, SemiDirection> map = {
      {SemiDirection::NORTH, SemiDirection::NORTHWEST},
      {SemiDirection::SOUTH, SemiDirection::SOUTHEAST},
      {SemiDirection::EAST, SemiDirection::NORTHEAST},
      {SemiDirection::WEST, SemiDirection::SOUTHWEST},
      {SemiDirection::NORTHEAST, SemiDirection::NORTH},
      {SemiDirection::NORTHWEST, SemiDirection::WEST},
      {SemiDirection::SOUTHEAST, SemiDirection::EAST},
      {SemiDirection::SOUTHWEST, SemiDirection::SOUTH},
  };
  return map;
}

const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_45_RIGHT() {
  static const QMap<SemiDirection, SemiDirection> map = {
      {SemiDirection::NORTH, SemiDirection::NORTHEAST},
      {SemiDirection::SOUTH, SemiDirection::SOUTHWEST},
      {SemiDirection::EAST, SemiDirection::SOUTHEAST},
      {SemiDirection::WEST, SemiDirection::NORTHWEST},
      {SemiDirection::NORTHEAST, SemiDirection::EAST},
      {SemiDirection::NORTHWEST, SemiDirection::NORTH},
      {SemiDirection::SOUTHEAST, SemiDirection::SOUTH},
      {SemiDirection::SOUTHWEST, SemiDirection::WEST},
  };
  return map;
}

const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_90_LEFT() {
  static const QMap<SemiDirection, SemiDirection> map = {
      {SemiDirection::NORTH, SemiDirection::WEST},
      {SemiDirection::SOUTH, SemiDirection::EAST},
      {SemiDirection::EAST, SemiDirection::NORTH},
      {SemiDirection::WEST, SemiDirection::SOUTH},
      {SemiDirection::NORTHEAST, SemiDirection::NORTHWEST},
      {SemiDirection::NORTHWEST, SemiDirection::SOUTHWEST},
      {SemiDirection::SOUTHEAST, SemiDirection::NORTHEAST},
      {SemiDirection::SOUTHWEST, SemiDirection::SOUTHEAST},
  };
  return map;
}

const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_90_RIGHT() {
  static const QMap<SemiDirection, SemiDirection> map = {
      {SemiDirection::NORTH, SemiDirection::EAST},
      {SemiDirection::SOUTH, SemiDirection::WEST},
      {SemiDirection::EAST, SemiDirection::SOUTH},
      {SemiDirection::WEST, SemiDirection::NORTH},
      {SemiDirection::NORTHEAST, SemiDirection::SOUTHEAST},
      {SemiDirection::NORTHWEST, SemiDirection::NORTHEAST},
      {SemiDirection::SOUTHEAST, SemiDirection::SOUTHWEST},
      {SemiDirection::SOUTHWEST, SemiDirection::NORTHWEST},
  };
  return map;
}

const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_180() {
  static const QMap<SemiDirection, SemiDirection> map = {
      {SemiDirection::NORTH, SemiDirection::SOUTH},
      {SemiDirection::SOUTH, SemiDirection::NORTH},
      {SemiDirection::EAST, SemiDirection::WEST},
      {SemiDirection::WEST, SemiDirection::EAST},
      {SemiDirection::NORTHEAST, SemiDirection::SOUTHWEST},
      {SemiDirection::NORTHWEST, SemiDirection::SOUTHEAST},
      {SemiDirection::SOUTHEAST, SemiDirection::NORTHWEST},
      {SemiDirection::SOUTHWEST, SemiDirection::NORTHEAST},
  };
  return map;
}

const QMap<QChar, Direction> &CHAR_TO_DIRECTION() {
  static const QMap<QChar, Direction> map = {
      {'n', Direction::NORTH},
      {'e', Direction::EAST},
      {'s', Direction::SOUTH},
      {'w', Direction::WEST},
  };
  return map;
}

const QMap<SemiDirection, Angle> &DIRECTION_TO_ANGLE() {
  static const QMap<SemiDirection, Angle> map = {
      {SemiDirection::EAST, Angle::Degrees(0)},
      {SemiDirection::NORTHEAST, Angle::Degrees(45)},
      {SemiDirection::NORTH, Angle::Degrees(90)},
      {SemiDirection::NORTHWEST, Angle::Degrees(135)},
      {SemiDirection::WEST, Angle::Degrees(180)},
      {SemiDirection::SOUTHWEST, Angle::Degrees(225)},
      {SemiDirection::SOUTH, Angle::Degrees(270)},
      {SemiDirection::SOUTHEAST, Angle::Degrees(315)},
  };
  return map;
}

}  // namespace mms
