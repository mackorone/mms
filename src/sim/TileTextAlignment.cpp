#include "TileTextAlignment.h"

#include "ContainerUtilities.h"


namespace mms {

const QMap<TileTextAlignment, QString>& TILE_TEXT_ALIGNMENT_TO_STRING() {
    static const QMap<TileTextAlignment, QString> map = {
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
    return map;
}

const QMap<QString, TileTextAlignment>& STRING_TO_TILE_TEXT_ALIGNMENT() {
    static const QMap<QString, TileTextAlignment> map =
        ContainerUtilities::inverse(TILE_TEXT_ALIGNMENT_TO_STRING());
    return map;
}

const QVector<TileTextAlignment>& UPPER_STAR_ALIGNMENTS() {
    static const QVector<TileTextAlignment> map = {
        TileTextAlignment::UPPER_LEFT, 
        TileTextAlignment::UPPER_CENTER, 
        TileTextAlignment::UPPER_RIGHT, 
    };
    return map;
}

const QVector<TileTextAlignment>& CENTER_STAR_ALIGNMENTS() {
    static const QVector<TileTextAlignment> map = {
        TileTextAlignment::CENTER_LEFT, 
        TileTextAlignment::CENTER_CENTER, 
        TileTextAlignment::CENTER_RIGHT, 
    };
    return map;
}

const QVector<TileTextAlignment>& LOWER_STAR_ALIGNMENTS() {
    static const QVector<TileTextAlignment> map = {
        TileTextAlignment::LOWER_LEFT, 
        TileTextAlignment::LOWER_CENTER, 
        TileTextAlignment::LOWER_RIGHT, 
    };
    return map;
}

const QVector<TileTextAlignment>& STAR_LEFT_ALIGNMENTS() {
    static const QVector<TileTextAlignment> map = {
        TileTextAlignment::UPPER_LEFT, 
        TileTextAlignment::CENTER_LEFT, 
        TileTextAlignment::LOWER_LEFT, 
    };
    return map;
}

const QVector<TileTextAlignment>& STAR_CENTER_ALIGNMENTS() {
    static const QVector<TileTextAlignment> map = {
        TileTextAlignment::UPPER_CENTER, 
        TileTextAlignment::CENTER_CENTER, 
        TileTextAlignment::LOWER_CENTER, 
    };
    return map;
}

const QVector<TileTextAlignment>& STAR_RIGHT_ALIGNMENTS() {
    static const QVector<TileTextAlignment> map = {
        TileTextAlignment::UPPER_RIGHT, 
        TileTextAlignment::CENTER_RIGHT, 
        TileTextAlignment::LOWER_RIGHT, 
    };
    return map;
}

} // namespace mms
