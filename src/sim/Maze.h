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

    // Initialize the object by reading a maze file; true if successful, false otherwise
    bool initializeViaMazeFile();

    // Initialize the Maze object by randomly generating a maze
    void initializeViaMazeGenerator();

    // Initializes all of the tiles of the maze
    void initializeMaze(const std::vector<std::vector<BasicTile>>& maze);

    // Extracts the maze information
    std::vector<std::vector<BasicTile>> extractMaze() const;
};

} // namespace sim
