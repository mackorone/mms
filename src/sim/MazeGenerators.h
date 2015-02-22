#pragma once

#include <string>
#include <utility>
#include <vector>

#include "BasicTile.h"

namespace sim {

std::vector<std::vector<BasicTile>> generateMaze(std::string algoName);

} // namespace sim
