#pragma once

#include <vector>

#include "Maze.h"
#include "TileGraphic.h"

namespace sim {

class MazeGraphic {

public:
    MazeGraphic(const Maze* maze);
    void draw() const;

    int getWidth() const;
    int getHeight() const;
    void setColor(int x, int y, const GLfloat* color);
    void setAlgoWall(int x, int y, Direction direction, bool isWall);

private:
    std::vector<std::vector<TileGraphic>> m_tileGraphics;
};

} // namespace sim
