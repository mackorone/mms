#pragma once

#include <QDebug>
#include <QMap>
#include <QString>
#include <QVector>

#include "ContainerUtilities.h"

namespace mms {

enum class TileTextAlignment {
     UPPER_LEFT,  UPPER_CENTER,  UPPER_RIGHT,
    CENTER_LEFT, CENTER_CENTER, CENTER_RIGHT,
     LOWER_LEFT,  LOWER_CENTER,  LOWER_RIGHT,
};

const QMap<TileTextAlignment, QString>& TILE_TEXT_ALIGNMENT_TO_STRING();
const QMap<QString, TileTextAlignment>& STRING_TO_TILE_TEXT_ALIGNMENT();

const QVector<TileTextAlignment>& UPPER_STAR_ALIGNMENTS();
const QVector<TileTextAlignment>& CENTER_STAR_ALIGNMENTS();
const QVector<TileTextAlignment>& LOWER_STAR_ALIGNMENTS();

const QVector<TileTextAlignment>& STAR_LEFT_ALIGNMENTS();
const QVector<TileTextAlignment>& STAR_CENTER_ALIGNMENTS();
const QVector<TileTextAlignment>& STAR_RIGHT_ALIGNMENTS();

inline QDebug operator<<(QDebug stream, TileTextAlignment tileTextAlignment) {
    stream.noquote() << TILE_TEXT_ALIGNMENT_TO_STRING().value(tileTextAlignment);
    return stream;
}

} // namespace mms
