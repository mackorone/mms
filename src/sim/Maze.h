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
    bool isValidMaze() const;
    bool isOfficialMaze() const;
    bool isCenterTile(int x, int y) const;

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;

    // Used for memoizing MazeChecker functions
    bool m_isValidMaze;
    bool m_isOfficialMaze;

    // Initializes all of the tiles of the basic maze
    static std::vector<std::vector<Tile>> initializeFromBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze);

    // Returns a basic maze of a particular width and height
    static std::vector<std::vector<BasicTile>> getBlankBasicMaze(int mazeWidth, int mazeHeight);

    // Basic maze geometric transformations
    static std::vector<std::vector<BasicTile>> mirrorAcrossVertical(const std::vector<std::vector<BasicTile>>& basicMaze);
    static std::vector<std::vector<BasicTile>> rotateCounterClockwise(const std::vector<std::vector<BasicTile>>& basicMaze);

    // (Re)set the distance values for the tiles in maze that are reachable from the center
    static std::vector<std::vector<Tile>> setTileDistances(std::vector<std::vector<Tile>> maze);
};

} // namespace sim
