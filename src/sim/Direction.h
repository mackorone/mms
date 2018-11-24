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

// This vector serves a few functions:
// 1) A convenient way to enumerate through all of the directions
// 2) Determines the order of wall information in .NUM maze files
// 3) Determines the location of wall information in the vertex buffer
const QVector<Direction>& DIRECTIONS();

const QMap<Direction, Direction>& DIRECTION_ROTATE_LEFT();
const QMap<Direction, Direction>& DIRECTION_ROTATE_RIGHT();
const QMap<Direction, Direction>& DIRECTION_OPPOSITE();

const QMap<Direction, Angle>& DIRECTION_TO_ANGLE();

const QMap<Direction, QString>& DIRECTION_TO_STRING();
const QMap<QString, Direction>& STRING_TO_DIRECTION();

const QMap<Direction, QChar>& DIRECTION_TO_CHAR();
const QMap<QChar, Direction>& CHAR_TO_DIRECTION();

inline QDebug operator<<(QDebug stream, Direction direction) {
    stream.noquote() << DIRECTION_TO_STRING().value(direction);
    return stream;
}

} 
