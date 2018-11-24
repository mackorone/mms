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

const QMap<MazeFileType, QString>& MAZE_FILE_TYPE_TO_STRING();
const QMap<QString, MazeFileType>& STRING_TO_MAZE_FILE_TYPE();

const QMap<MazeFileType, QString>& MAZE_FILE_TYPE_TO_SUFFIX();
const QMap<QString, MazeFileType>& SUFFIX_TO_MAZE_FILE_TYPE();

inline QDebug operator<<(QDebug stream, MazeFileType mazeFileType) {
    stream.noquote() << MAZE_FILE_TYPE_TO_STRING().value(mazeFileType);
    return stream;
}

} 
