#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeChecker {

public:

    // The MazeChecker class is not constructible
    MazeChecker() = delete;

    // Returns true is a maze is valid (usable by the simulator), false otherwise
    static bool isValidMaze(const std::vector<std::vector<BasicTile>>& maze);

    // Returns true if a maze complies with the official rules, false otherwise
    static bool isOfficialMaze(const std::vector<std::vector<BasicTile>>& maze);

    // Misc. helper function, used by Maze
    static std::vector<std::pair<int, int>> getCenterTiles(int width, int height);

private:

    // isValidMaze helper functions
    static bool isNonempty(const std::vector<std::vector<BasicTile>>& maze);
    static bool isRectangular(const std::vector<std::vector<BasicTile>>& maze);
    static bool isEnclosed(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasConsistentWalls(const std::vector<std::vector<BasicTile>>& maze);

    // isOfficialMaze helper functions
    static bool hasNoInaccessibleLocations(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasThreeStartingWalls(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasOneEntranceToCenter(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasHollowCenter(const std::vector<std::vector<BasicTile>>& maze);
    static bool hasWallAttachedToEachNonCenterPost(const std::vector<std::vector<BasicTile>>& maze);
    static bool isUnsolvableByWallFollower(const std::vector<std::vector<BasicTile>>& maze);

    // Misc. helper functions
    static Direction directionAfterLeftTurn(Direction direction);
    static Direction directionAfterRightTurn(Direction direction);
    static std::pair<int, int> positionAfterMovingForward(std::pair<int, int> position, Direction direction);

};

} // namespace sim
