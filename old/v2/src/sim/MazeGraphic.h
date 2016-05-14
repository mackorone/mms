#pragma once

#include <vector>

#include "BufferInterface.h"
#include "Color.h"
#include "Maze.h"
#include "TileGraphic.h"

namespace sim {

class MazeGraphic {

public:
    MazeGraphic(const Maze* maze, BufferInterface* bufferInterface);

    void setTileColor(int x, int y, Color color);
    void declareWall(int x, int y, Direction direction, bool isWall);
    void undeclareWall(int x, int y, Direction direction);
    void setTileFogginess(int x, int y, bool foggy);
    void setTileText(int x, int y, const std::vector<std::string>& rowsOfText);

    void draw() const;
    void updateColor() const;
    void updateWalls() const;
    void updateFog() const;
    void updateText() const;

private:
    std::vector<std::vector<TileGraphic>> m_tileGraphics;

    int getWidth() const;
    int getHeight() const;
    bool withinMaze(int x, int y) const;

};

} // namespace sim
