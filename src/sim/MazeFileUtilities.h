#pragma once

#include <string>
#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeFileUtilities {

public:

    // Returns whether or not the contents of the file have the proper format
    static bool isMazeFile(const std::string& mazeFilePath);

    // Functions for saving and loading the maze
    static void saveMaze(const std::vector<std::vector<BasicTile>>& maze, const std::string& mazeFilePath);
    static std::vector<std::vector<BasicTile>> loadMaze(const std::string& mazeFilePath);

private:

    // A private constructor to restrict creation of any MazeFileUtilities objects
    MazeFileUtilities();

    // Returns the maze width and height (in that order)
    static std::pair<int, int> getMazeSize(const std::string& mazeFilePath);

};

} // namespace sim
