#pragma once

#include <string>
#include <vector>

#include "BasicTile.h"
#include "Tile.h"

namespace sim {

class Maze {

public:
    Maze();
    int getWidth() const;
    int getHeight() const;
    bool withinMaze(int x, int y) const;
    Tile* getTile(int x, int y);
    const Tile* getTile(int x, int y) const;

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;

    // Initializes all of the tiles of the basic maze
    std::vector<std::vector<Tile>> initializeFromBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze) const;

    // Returns a basic maze of a particular width and height
    std::vector<std::vector<BasicTile>> getBlankBasicMaze(int mazeWidth, int mazeHeight) const;

    // Basic maze geometric transformations
    std::vector<std::vector<BasicTile>> mirrorAcrossVertical(const std::vector<std::vector<BasicTile>>& basicMaze) const;
    std::vector<std::vector<BasicTile>> rotateCounterClockwise(const std::vector<std::vector<BasicTile>>& basicMaze) const;

    // (Re)set the distance values for the tiles in maze that are reachable from the center
    std::vector<std::vector<Tile>> setTileDistances(std::vector<std::vector<Tile>> maze) const;
};

} // namespace sim
