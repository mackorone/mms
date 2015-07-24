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
    bool initializeViaMazeFile(const std::string& mazeFilePath);

    // Initialize the object via a maze generation algorithm; true if successful, false otherwise
    bool initializeViaMazeAlgorithm();

    // Perform validation on the given basic maze
    bool validateBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze);

    // Initializes all of the tiles of the basic maze
    void initializeFromBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze);

    // Extracts all basic maze information
    std::vector<std::vector<BasicTile>> extractBasicMaze() const;

    // Returns a basic maze of a particular width and height
    std::vector<std::vector<BasicTile>> getBlankBasicMaze(int mazeWidth, int mazeHeight) const;
};

} // namespace sim
