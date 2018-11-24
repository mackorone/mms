#include "MazeFileType.h"

#include "ContainerUtilities.h"

namespace mms {

const QMap<MazeFileType, QString>& MAZE_FILE_TYPE_TO_STRING() {
    static const QMap<MazeFileType, QString> map = {
        {MazeFileType::MAP, "MAP"},
        {MazeFileType::MAZ, "MAZ"},
        {MazeFileType::MZ2, "MZ2"},
        {MazeFileType::NUM, "NUM"},
    };
    return map;
}

const QMap<QString, MazeFileType>& STRING_TO_MAZE_FILE_TYPE() {
    static const QMap<QString, MazeFileType> map =
        ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING());
    return map;
}

const QMap<MazeFileType, QString>& MAZE_FILE_TYPE_TO_SUFFIX() {
    static const QMap<MazeFileType, QString> map = {
        {MazeFileType::MAP, "map"},
        {MazeFileType::MAZ, "MAZ"},
        {MazeFileType::MZ2, "MZ2"},
        {MazeFileType::NUM, "num"},
    };
    return map;
}

const QMap<QString, MazeFileType>& SUFFIX_TO_MAZE_FILE_TYPE() {
    static const QMap<QString, MazeFileType> map =
        ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING());
    return map;
}

} 
