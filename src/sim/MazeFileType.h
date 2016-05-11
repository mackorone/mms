#pragma once

#include <map>

namespace sim {

enum class MazeFileType { BIN, MAP, NUM };

static const std::map<MazeFileType, std::string> MAZE_FILE_TYPE_TO_STRING {
    {MazeFileType::BIN, "BIN"},
    {MazeFileType::MAP, "MAP"},
    {MazeFileType::NUM, "NUM"},
};

} // namespace sim
