#pragma once

#include <string>
#include <utility>

namespace sim {

std::pair<int, int> getMazeSize(std::string mazeFilePath);
bool validMaze(std::string mazeFilePath);

} // namespace sim
