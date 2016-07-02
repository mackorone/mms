#pragma once

// TODO: MACK - convert to QT
#include <string>
#include <vector>

#include "BasicMaze.h"
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
    static std::vector<std::vector<Tile>> initializeFromBasicMaze(const BasicMaze& basicMaze);

    // Basic maze geometric transformations
    static BasicMaze mirrorAcrossVertical(const BasicMaze& basicMaze);
    static BasicMaze rotateCounterClockwise(const BasicMaze& basicMaze);

    // (Re)set the distance values for the tiles in maze that are reachable from the center
    static std::vector<std::vector<Tile>> setTileDistances(std::vector<std::vector<Tile>> maze);
};

} // namespace sim
