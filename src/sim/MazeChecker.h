#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeChecker {

public:

    // The MazeChecker class is not constructible
    MazeChecker() = delete;

    // Returns true is a maze is valid (usable by the simulator), false otherwise
    static bool validMaze(const std::vector<std::vector<BasicTile>>& maze);

    // validMaze helper functions
    static bool isNonempty(const std::vector<std::vector<BasicTile>>& maze);
    static bool isRectangular(const std::vector<std::vector<BasicTile>>& maze);
    static bool isEnclosed(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasConsistentWalls(const std::vector<std::vector<BasicTile>>& maze);

    // Returns true if a maze complies with the official rules, false otherwise
    static bool officialMaze(const std::vector<std::vector<BasicTile>>& maze);

    // officialMaze helper functions
    static bool is16x16(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasPathToCenter(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasHollowCenter(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasOneEntranceToCenter(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasWallAttachedToEachPost(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasThreeStartingWalls(const std::vector<std::vector<BasicTile>>& maze);
    static bool isUnsolvableByWallFollower(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasNoInaccesibleLocations(const std::vector<std::vector<BasicTile>>& maze);

};

} // namespace sim
