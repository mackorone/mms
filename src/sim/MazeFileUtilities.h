#pragma once

#include <string>
#include <utility>
#include <vector>

#include "BasicTile.h"

namespace sim {

std::pair<int, int> getMazeSize(std::string mazeFilePath);
bool validMaze(std::string mazeFilePath);
bool officialMaze(std::string mazeFilePath);
bool hasPathToCenter(const std::vector<std::vector<BasicTile>> &maze, int x, int y);
bool hasOneEntrance(const std::vector<std::vector<BasicTile>> &maze);
bool eachPostHasWall(const std::vector<std::vector<BasicTile>> &maze, int x, int y);
bool threeStartingWalls(const std::vector<std::vector<BasicTile>> &maze);
void saveMaze(std::vector<std::vector<BasicTile>> maze, std::string mazeFilePath);
std::vector<std::vector<BasicTile>> loadMaze(std::string mazeFilePath);

} // namespace sim
