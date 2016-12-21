#pragma once

#include <QDebug>
#include <QMap>
#include <QString>

#include "ContainerUtilities.h"

namespace mms {

enum class MazeFileType {
    MAP,
    MAZ,
    MZ2,
    NUM,
};

static const QMap<MazeFileType, QString> MAZE_FILE_TYPE_TO_STRING {
    {MazeFileType::MAP, "MAP"},
    {MazeFileType::MAZ, "MAZ"},
    {MazeFileType::MZ2, "MZ2"},
    {MazeFileType::NUM, "NUM"},
};

static const QMap<QString, MazeFileType> STRING_TO_MAZE_FILE_TYPE =
    ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING);

static const QMap<MazeFileType, QString> MAZE_FILE_TYPE_TO_SUFFIX {
    {MazeFileType::MAP, "map"},
    {MazeFileType::MAZ, "MAZ"},
    {MazeFileType::MZ2, "MZ2"},
    {MazeFileType::NUM, "num"},
};

static const QMap<QString, MazeFileType> SUFFIX_TO_MAZE_FILE_TYPE =
    ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING);

inline QDebug operator<<(QDebug stream, MazeFileType mazeFileType) {
    stream.noquote() << MAZE_FILE_TYPE_TO_STRING.value(mazeFileType);
    return stream;
}

} // namespace mms
