#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeInterface {

public:
    MazeInterface(std::vector<std::vector<BasicTile>>* basicMaze);

private:
    std::vector<std::vector<BasicTile>>* m_basicMaze;

};

} // namespace sim
