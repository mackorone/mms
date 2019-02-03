#pragma once

#include <QDebug>
#include <QMap>
#include <QString>
#include <QVector>

#include "units/Angle.h"

namespace mms {

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

// This vector serves two functions:
// 1) A convenient way to enumerate through all of the directions
// 2) Determines the location of wall information in the vertex buffer
const QVector<Direction>& DIRECTIONS();

const QMap<Direction, Direction>& DIRECTION_ROTATE_LEFT();
const QMap<Direction, Direction>& DIRECTION_ROTATE_RIGHT();

const QMap<QChar, Direction>& CHAR_TO_DIRECTION();
const QMap<Direction, Angle>& DIRECTION_TO_ANGLE();

} 
