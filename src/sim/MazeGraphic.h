#pragma once

#include <vector>

#include "Colors.h"
#include "Maze.h"
#include "TileGraphic.h"

namespace sim {

class MazeGraphic {

public:
    MazeGraphic(const Maze* maze);

    int getWidth() const;
    int getHeight() const;

    bool wallDeclared(int x, int y, Direction direction) const;

    void setTileColor(int x, int y, Color color);
    void setTileDistance(int x, int y, int distance);
    void setTileFogginess(int x, int y, bool foggy);
    void declareWall(int x, int y, Direction direction, bool isWall);
    void undeclareWall(int x, int y, Direction direction);

    void draw() const;
    void updateColor() const;
    void updateWalls() const;
    void updateFog() const;

private:
    std::vector<std::vector<TileGraphic>> m_tileGraphics;
};

} // namespace sim
