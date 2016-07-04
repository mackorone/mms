#pragma once

#include <map>

#include "ContainerUtilities.h"

namespace sim {

enum class MazeFileType {
    MAP,
    MAZ,
    MZ2,
    NUM // TODO: MACK - comma here
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
    {MazeFileType::MAP, ".map"},
    {MazeFileType::MAZ, ".MAZ"},
    {MazeFileType::MZ2, ".MZ2"},
    {MazeFileType::NUM, ".num"},
};

static const QMap<QString, MazeFileType> SUFFIX_TO_MAZE_FILE_TYPE =
    ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING);

} // namespace sim
