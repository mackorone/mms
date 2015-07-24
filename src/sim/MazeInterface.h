#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeInterface {

public:
    MazeInterface(std::vector<std::vector<BasicTile>>* basicMaze);

    // TODO: MACK - complete the interface
    void setWall(int x, int y, char direction, bool wallExists);

private:
    std::vector<std::vector<BasicTile>>* m_basicMaze;

};

} // namespace sim
