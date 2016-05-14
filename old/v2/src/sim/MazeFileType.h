#pragma once

#include <map>

#include "ContainerUtilities.h"

namespace sim {

enum class MazeFileType { MAP, NUM };

static const std::map<MazeFileType, std::string> MAZE_FILE_TYPE_TO_STRING {
    {MazeFileType::MAP, "MAP"},
    {MazeFileType::NUM, "NUM"},
};

static const std::map<std::string, MazeFileType> STRING_TO_MAZE_FILE_TYPE =
    ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING);

static const std::map<MazeFileType, std::string> MAZE_FILE_TYPE_TO_SUFFIX {
    {MazeFileType::MAP, ".map"},
    {MazeFileType::NUM, ".num"},
};

static const std::map<std::string, MazeFileType> SUFFIX_TO_MAZE_FILE_TYPE =
    ContainerUtilities::inverse(MAZE_FILE_TYPE_TO_STRING);

} // namespace sim
