#pragma once

#include <QPair>
#include <QSet>
#include <QString>
#include <QVector>

#include "BasicMaze.h"

namespace mms {

class MazeChecker {

public:

    // The MazeChecker class is not constructible
    MazeChecker() = delete;

    // Whether or not the maze is:
    // - Drawable: can be rendered without crashing
    // - Valid: a valid maze model, consistent and enclosed
    // - Official: complies with the official competition rules
    // Returns success and a list of errors/failures
    static QPair<bool, QVector<QString>> isDrawableMaze(const BasicMaze& maze);
    static QPair<bool, QVector<QString>> isValidMaze(const BasicMaze& maze);
    static QPair<bool, QVector<QString>> isOfficialMaze(const BasicMaze& maze);

    // TODO: MACK - this should go somewhere else
    // Misc. helper function, used by Maze
    static QSet<QPair<int, int>> getCenterTiles(int width, int height);

private:

    // These return a list of errors - empty means success
    static QVector<QString> isNonempty(const BasicMaze& maze);
    static QVector<QString> isRectangular(const BasicMaze& maze);
    static QVector<QString> isEnclosed(const BasicMaze& maze);
    static QVector<QString> hasConsistentWalls(const BasicMaze& maze);
    static QVector<QString> hasNoInaccessibleLocations(const BasicMaze& maze);
    static QVector<QString> hasThreeStartingWalls(const BasicMaze& maze);
    static QVector<QString> hasOneEntranceToCenter(const BasicMaze& maze);
    static QVector<QString> hasHollowCenter(const BasicMaze& maze);
    static QVector<QString> hasWallAttachedToEachNonCenterPost(const BasicMaze& maze);
    static QVector<QString> isUnsolvableByWallFollower(const BasicMaze& maze);

    // TODO: MACK - this should go somewhere else too - MazeUtilities.h
    // Misc. helper function
    static QPair<int, int> positionAfterMovingForward(
        QPair<int, int> position,
        Direction direction);
};

} // namespace mms
