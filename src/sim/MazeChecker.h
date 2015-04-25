#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeChecker {

public:

    // Returns true is a maze is valid (usable by the simulator), false otherwise
    static bool validMaze(const std::vector<std::vector<BasicTile>>& maze);

    // Returns true if a maze complies with the official rules, false otherwise
    static bool officialMaze(const std::vector<std::vector<BasicTile>>& maze);

private:

    // A private constructor to restrict creation of any MazeChecker objects
    MazeChecker();

    // validMaze helper functions
    static bool isSquare(const std::vector<std::vector<BasicTile>>& maze);
    static bool isEnclosed(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasConsistentWalls(const std::vector<std::vector<BasicTile>>& maze);

    // officialMaze helper functions
    static bool hasPathToCenter(const std::vector<std::vector<BasicTile>>& maze, int x, int y);
    static bool hasOneEntrance(const std::vector<std::vector<BasicTile>>& maze);
    static bool eachPostHasWall(const std::vector<std::vector<BasicTile>>& maze, int x, int y);
    static bool threeStartingWalls(const std::vector<std::vector<BasicTile>>& maze);
    static bool unsolvableByWallFollower(const std::vector<std::vector<BasicTile>>& maze);
    static bool noInaccesibleLocations(const std::vector<std::vector<BasicTile>>& maze);

};

} // namespace sim
