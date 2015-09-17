#pragma once

#include <vector>

#include "Color.h"
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
};

} // namespace sim
