#pragma once

#include <QMap>
#include <QVector>

#include "units/Angle.h"

namespace mms {

enum class Direction {
  NORTH,
  EAST,
  SOUTH,
  WEST,
};

enum class SemiDirection {
  NORTH,
  SOUTH,
  EAST,
  WEST,
  NORTHEAST,
  SOUTHEAST,
  NORTHWEST,
  SOUTHWEST,
};

// This vector serves two functions:
// 1) A convenient way to enumerate through all of the directions
// 2) Determines the location of wall information in the vertex buffer
const QVector<Direction> &CARDINAL_DIRECTIONS();

const QVector<SemiDirection> &ORDINAL_DIRECTIONS();
const QMap<SemiDirection, Direction> &SEMI_TO_CARDINAL();

const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_45_LEFT();
const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_45_RIGHT();
const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_90_LEFT();
const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_90_RIGHT();
const QMap<SemiDirection, SemiDirection> &DIRECTION_ROTATE_180();

const QMap<QChar, Direction> &CHAR_TO_DIRECTION();
const QMap<SemiDirection, Angle> &DIRECTION_TO_ANGLE();

}  // namespace mms
