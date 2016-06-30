#pragma once

#include <QVector>

#include "BasicTile.h"

namespace sim {

class MazeChecker {

public:

    // The MazeChecker class is not constructible
    MazeChecker() = delete;

    // Returns true is a maze is valid (usable by the simulator), false otherwise
    static bool isValidMaze(const QVector<QVector<BasicTile>>& maze);

    // Returns true if a maze complies with the official rules, false otherwise
    static bool isOfficialMaze(const QVector<QVector<BasicTile>>& maze);

    // Misc. helper function, used by Maze
    static QVector<std::pair<int, int>> getCenterTiles(int width, int height);

private:

    // isValidMaze helper functions
    static bool isNonempty(const QVector<QVector<BasicTile>>& maze);
    static bool isRectangular(const QVector<QVector<BasicTile>>& maze);
    static bool isEnclosed(const QVector<QVector<BasicTile>>& maze);
    static bool hasConsistentWalls(const QVector<QVector<BasicTile>>& maze);

    // isOfficialMaze helper functions
    static bool hasNoInaccessibleLocations(const QVector<QVector<BasicTile>>& maze);
    static bool hasThreeStartingWalls(const QVector<QVector<BasicTile>>& maze);
    static bool hasOneEntranceToCenter(const QVector<QVector<BasicTile>>& maze);
    static bool hasHollowCenter(const QVector<QVector<BasicTile>>& maze);
    static bool hasWallAttachedToEachNonCenterPost(const QVector<QVector<BasicTile>>& maze);
    static bool isUnsolvableByWallFollower(const QVector<QVector<BasicTile>>& maze);

    // Misc. helper functions
    static Direction directionAfterLeftTurn(Direction direction);
    static Direction directionAfterRightTurn(Direction direction);
    static std::pair<int, int> positionAfterMovingForward(std::pair<int, int> position, Direction direction);

};

} // namespace sim
