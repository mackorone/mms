#pragma once

#include <vector>

#include "Maze.h"
#include "TileGraphic.h"

namespace sim {

class MazeGraphic {

public:
    MazeGraphic(const Maze* maze);
    void draw();

private:
    std::vector<std::vector<TileGraphic>> m_tileGraphics;
};

} // namespace sim
