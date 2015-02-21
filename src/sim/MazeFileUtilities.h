#pragma once

#include <string>
#include <utility>
#include <vector>

#include "BasicTile.h"

namespace sim {

std::pair<int, int> getMazeSize(std::string mazeFilePath);
bool validMaze(std::string mazeFilePath);
bool officialMaze(std::string mazeFilePath);
void saveMaze(std::string mazeFile, std::vector<std::vector<BasicTile>> maze);
std::vector<std::vector<BasicTile>> loadMaze(std::string mazeFile);

} // namespace sim
