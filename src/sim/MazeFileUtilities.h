#pragma once

#include <string>
#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeFileUtilities {

public:

    MazeFileUtilities() = delete;

    static bool isMazeFile(const std::string& mazeFilePath);
    static void saveMaze(const std::vector<std::vector<BasicTile>>& maze, const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMaze(const std::string& mazeFilePath);

    // TODO: MACK - rename these
    static bool isMapFile(const std::string& mapFilePath);
    static void saveMap(const std::vector<std::vector<BasicTile>>& maze, const std::string& mapFilePath);
    static std::vector<std::vector<BasicTile>> loadMap(const std::string& mapFilePath);

};

} // namespace sim
