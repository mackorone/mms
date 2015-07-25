#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeInterface {

public:
    MazeInterface(std::vector<std::vector<BasicTile>>* basicMaze);

    void setWall(int x, int y, char direction, bool wallExists);
    double getRandom();

private:
    std::vector<std::vector<BasicTile>>* m_basicMaze;
    int getWidth();
    int getHeight();

};

} // namespace sim
