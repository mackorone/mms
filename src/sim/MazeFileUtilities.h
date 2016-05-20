#pragma once

#include <string>
#include <vector>

#include "BasicTile.h"
#include "MazeFileType.h"

namespace sim {

class MazeFileUtilities {

public:

    MazeFileUtilities() = delete;

    static std::vector<std::vector<BasicTile>> loadMaze(
        const std::string& mazeFilePath);

    static bool saveMaze(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath,
        MazeFileType mazeFileType);

private:

    static std::vector<std::vector<BasicTile>> loadMazeFileMapType(
        const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMazeFileMazType(
        const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMazeFileMz2Type(
        const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMazeFileNumType(
        const std::string& mazeFilePath);

    static bool saveMazeFileMapType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);
    static bool saveMazeFileMazType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);
    static bool saveMazeFileMz2Type(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);
    static bool saveMazeFileNumType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mapFilePath);

};

} // namespace sim
