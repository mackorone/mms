#pragma once

#include <string>
#include <utility>
#include <vector>

#include "BasicTile.h"

namespace sim {

// Returns the maze width and height (in that order)
std::pair<int, int> getMazeSize(const std::string& mazeFilePath);

// Returns true is a maze is valid (usable by the simulator), false otherwise
bool validMaze(const std::string& mazeFilePath);

// Returns true if a maze complies with the official rules, false otherwise
bool officialMaze(const std::string& mazeFilePath);

// officialMaze helper functions
bool hasPathToCenter(const std::vector<std::vector<BasicTile>>& maze, int x, int y);
bool hasOneEntrance(const std::vector<std::vector<BasicTile>>& maze);
bool eachPostHasWall(const std::vector<std::vector<BasicTile>>& maze, int x, int y);
bool threeStartingWalls(const std::vector<std::vector<BasicTile>>& maze);

// Functions for saving and loading the maze
void saveMaze(const std::vector<std::vector<BasicTile>>& maze, const std::string& mazeFilePath);
std::vector<std::vector<BasicTile>> loadMaze(const std::string& mazeFilePath);

} // namespace sim
