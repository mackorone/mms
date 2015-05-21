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
    void setTileColor(int x, int y, const GLfloat* color);
    void setTileText(int x, int y, const std::string& text);
    void declareWall(int x, int y, Direction direction, bool isWall);
    void undeclareWall(int x, int y, Direction direction);

private:
    std::vector<std::vector<TileGraphic>> m_tileGraphics;
};

} // namespace sim
