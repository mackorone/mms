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
    Tile* getTile(int x, int y);
    const Tile* getTile(int x, int y) const;

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;

    // Initializes all of the tiles of the basic maze
    void initializeFromBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze);

    // Extracts all basic maze information
    std::vector<std::vector<BasicTile>> extractBasicMaze() const;

    // Returns a basic maze of a particular width and height
    std::vector<std::vector<BasicTile>> getBlankBasicMaze(int mazeWidth, int mazeHeight) const;

    // Basic maze geometric transformations
    std::vector<std::vector<BasicTile>> mirrorAcrossVertical(const std::vector<std::vector<BasicTile>>& basicMaze);
    std::vector<std::vector<BasicTile>> rotate90DegreesClockwise(const std::vector<std::vector<BasicTile>>& basicMaze);
};

} // namespace sim
