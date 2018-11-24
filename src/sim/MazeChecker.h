#pragma once

#include "BasicMaze.h"

namespace mms {

enum class MazeValidity {
    INVALID,
    // Maze can be rendered
    DRAWABLE,
    // Maze be explored by a mouse
    EXPLORABLE,
    // Maze follows official guidelines
    OFFICIAL,
};

class MazeChecker {

public:

    MazeChecker() = delete;
    static MazeValidity checkMaze(const BasicMaze& maze);

private:

    static bool isNonempty(const BasicMaze& maze);
    static bool isRectangular(const BasicMaze& maze);
    static bool isEnclosed(const BasicMaze& maze);
    static bool hasConsistentWalls(const BasicMaze& maze);
    static bool hasNoInaccessibleLocations(const BasicMaze& maze);
    static bool hasThreeStartingWalls(const BasicMaze& maze);
    static bool hasOneEntranceToCenter(const BasicMaze& maze);
    static bool hasHollowCenter(const BasicMaze& maze);
    static bool hasWallAttachedToEachNonCenterPost(const BasicMaze& maze);
    static bool isUnsolvableByWallFollower(const BasicMaze& maze);

};

} 
