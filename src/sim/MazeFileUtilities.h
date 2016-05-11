#pragma once

#include <map>
#include <string>
#include <vector>

#include "BasicTile.h"
#include "MazeFileType.h"

namespace sim {

class MazeFileUtilities {

public:

    MazeFileUtilities() = delete;

    // Returns a vector of vectors of error messages. If empty, it means
    // that the mazeFilePath is a valid maze file. If not, it's invalid.
    static std::map<MazeFileType, std::vector<std::string>> checkMazeFile(
        const std::string& mazeFilePath);

    static void saveMaze(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath,
        MazeFileType mazeFileType);

    static std::vector<std::vector<BasicTile>> loadMaze(
        const std::string& mazeFilePath);

private:

    static std::vector<std::string> checkMazeFileBinType(
        const std::string& mazeFilePath);
    static std::vector<std::string> checkMazeFileMapType(
        const std::string& mazeFilePath);
    static std::vector<std::string> checkMazeFileNumType(
        const std::string& mazeFilePath);

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
