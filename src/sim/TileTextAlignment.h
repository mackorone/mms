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

static const QMap<TileTextAlignment, QString> TILE_TEXT_ALIGNMENT_TO_STRING {
    {TileTextAlignment::UPPER_LEFT, "UPPER_LEFT"},
    {TileTextAlignment::UPPER_CENTER, "UPPER_CENTER"},
    {TileTextAlignment::UPPER_RIGHT, "UPPER_RIGHT"},
    {TileTextAlignment::CENTER_LEFT, "CENTER_LEFT"},
    {TileTextAlignment::CENTER_CENTER, "CENTER_CENTER"},
    {TileTextAlignment::CENTER_RIGHT, "CENTER_RIGHT"},
    {TileTextAlignment::LOWER_LEFT, "LOWER_LEFT"},
    {TileTextAlignment::LOWER_CENTER, "LOWER_CENTER"},
    {TileTextAlignment::LOWER_RIGHT, "LOWER_RIGHT"},
};

static const QMap<QString, TileTextAlignment> STRING_TO_TILE_TEXT_ALIGNMENT =
    ContainerUtilities::inverse(TILE_TEXT_ALIGNMENT_TO_STRING);

static const QVector<TileTextAlignment> UPPER_STAR_ALIGNMENTS {
    TileTextAlignment::UPPER_LEFT, 
    TileTextAlignment::UPPER_CENTER, 
    TileTextAlignment::UPPER_RIGHT, 
};

static const QVector<TileTextAlignment> CENTER_STAR_ALIGNMENTS {
    TileTextAlignment::CENTER_LEFT, 
    TileTextAlignment::CENTER_CENTER, 
    TileTextAlignment::CENTER_RIGHT, 
};

static const QVector<TileTextAlignment> LOWER_STAR_ALIGNMENTS {
    TileTextAlignment::LOWER_LEFT, 
    TileTextAlignment::LOWER_CENTER, 
    TileTextAlignment::LOWER_RIGHT, 
};

static const QVector<TileTextAlignment> STAR_LEFT_ALIGNMENTS {
    TileTextAlignment::UPPER_LEFT, 
    TileTextAlignment::CENTER_LEFT, 
    TileTextAlignment::LOWER_LEFT, 
};

static const QVector<TileTextAlignment> STAR_CENTER_ALIGNMENTS {
    TileTextAlignment::UPPER_CENTER, 
    TileTextAlignment::CENTER_CENTER, 
    TileTextAlignment::LOWER_CENTER, 
};

static const QVector<TileTextAlignment> STAR_RIGHT_ALIGNMENTS {
    TileTextAlignment::UPPER_RIGHT, 
    TileTextAlignment::CENTER_RIGHT, 
    TileTextAlignment::LOWER_RIGHT, 
};

inline QDebug operator<<(QDebug stream, TileTextAlignment tileTextAlignment) {
    stream.noquote() << TILE_TEXT_ALIGNMENT_TO_STRING.value(tileTextAlignment);
    return stream;
}

} // namespace mms
