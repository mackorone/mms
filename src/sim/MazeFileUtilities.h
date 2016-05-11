#pragma once

#include <string>
#include <vector>

#include "BasicTile.h"
#include "MazeFileType.h"

namespace sim {

class MazeFileUtilities {

public:

    MazeFileUtilities() = delete;

    // TODO: MACK Return a vector of errors, if empty then yes
    static bool isMazeFile(const std::string& mazeFilePath);

    static void saveMaze(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath,
        MazeFileType mazeFileType);

    static std::vector<std::vector<BasicTile>> loadMaze(
        const std::string& mazeFilePath);

private:

    static bool isMazeFileBinType(const std::string& mazeFilePath);
    static bool isMazeFileMapType(const std::string& mazeFilePath);
    static bool isMazeFileNumType(const std::string& mazeFilePath);

    static void saveMazeFileBinType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);
    static void saveMazeFileMapType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);
    static void saveMazeFileNumType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);

    static std::vector<std::vector<BasicTile>> loadMazeFileBinType(
        const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMazeFileMapType(
        const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMazeFileNumType(
        const std::string& mazeFilePath);

};

} // namespace sim
