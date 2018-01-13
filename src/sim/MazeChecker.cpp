#include "MazeChecker.h"

#include <QPair>
#include <QQueue>

#include "Direction.h"
#include "MazeUtilities.h"

namespace mms {

MazeValidity MazeChecker::checkMaze(const BasicMaze& maze) {
    bool drawable =  (
        isNonempty(maze) &&
        isRectangular(maze)
    );
    if (!drawable) {
        return MazeValidity::INVALID;
    }
    bool explorable = (
        isEnclosed(maze) &&
        hasConsistentWalls(maze)
    );
    if (!explorable) {
        return MazeValidity::DRAWABLE;
    }
    bool official = (
        hasNoInaccessibleLocations(maze) &&
        hasThreeStartingWalls(maze) &&
        hasOneEntranceToCenter(maze) &&
        hasHollowCenter(maze) &&
        hasWallAttachedToEachNonCenterPost(maze) &&
        isUnsolvableByWallFollower(maze)
    );
    if (!official) {
        return MazeValidity::EXPLORABLE;
    }
    return MazeValidity::OFFICIAL;
}

bool MazeChecker::isNonempty(const BasicMaze& maze) {
    return (
        0 < maze.size() &&
        0 < maze.at(0).size()
    );
}

bool MazeChecker::isRectangular(const BasicMaze& maze) {
    for (int i = 0; i < maze.size() - 1; i += 1) {
        if (maze.at(i).size() != maze.at(i + 1).size()) {
            return false;
        }
    }
    return true;
}

bool MazeChecker::isEnclosed(const BasicMaze& maze) {
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            BasicTile walls = maze.at(x).at(y);
            if (x == 0 && !walls.value(Direction::WEST)) {
                return false;
            }
            if (y == 0 && !walls.value(Direction::SOUTH)) {
                return false;
            }
            if (x == maze.size() - 1 && !walls.value(Direction::EAST)) {
                return false;
            }
            if (y == maze.at(x).size() - 1 && !walls.value(Direction::NORTH)) {
                return false;
            }
        }
    }
    return true;
}

bool MazeChecker::hasConsistentWalls(const BasicMaze& maze) {
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            BasicTile walls = maze.at(x).at(y);
            if (
                0 < x &&
                walls.value(Direction::WEST) &&
                !maze.at(x - 1).at(y).value(Direction::EAST)
            ) {
                return false;
            }
            if (
                0 < y &&
                walls.value(Direction::SOUTH) &&
                !maze.at(x).at(y - 1).value(Direction::NORTH)
            ) {
                return false;
            }
            if (
                x < maze.size() - 1 &&
                walls.value(Direction::EAST) &&
                !maze.at(x + 1).at(y).value(Direction::WEST)
            ) {
                return false;
            }
            if (
                y < maze.at(x).size() - 1 &&
                walls.value(Direction::NORTH) &&
                !maze.at(x).at(y + 1).value(Direction::SOUTH)
            ) {
                return false;
            }
        }
    }
    return true;
}

bool MazeChecker::hasNoInaccessibleLocations(const BasicMaze& maze) {
    QSet<QPair<int, int>> discovered;
    QQueue<QPair<int, int>> queue;
    QPair<int, int> start = {0, 0};
    discovered.insert(start);
    queue.enqueue(start);
    while (!queue.isEmpty()) {
        QPair<int, int> tile = queue.dequeue();
        for (Direction direction : DIRECTIONS()) {
            if (maze.at(tile.first).at(tile.second).value(direction)) {
                continue;
            }
            QPair<int, int> neighbor =
                MazeUtilities::positionAfterMovingForward(tile, direction);
            if (!discovered.contains(neighbor)) {
                discovered.insert(neighbor);
                queue.enqueue(neighbor);
            }
        }
    }
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            if (!discovered.contains({x, y})) {
                return false;
            }
        }
    }
    return true;
}

bool MazeChecker::hasThreeStartingWalls(const BasicMaze& maze) {
    int count = 0;
    for (Direction direction : DIRECTIONS()) {
        if (maze.at(0).at(0).value(direction)) {
            count += 1;
        }
    }
    return count == 3;
}

bool MazeChecker::hasOneEntranceToCenter(const BasicMaze& maze) {
    const auto& centerPositions =
        MazeUtilities::getCenterPositions(maze.size(), maze.at(0).size()); 
    int numberOfEntrances = 0;
    for (QPair<int, int> tile : centerPositions) {
        for (Direction direction : DIRECTIONS()) {
            if (centerPositions.contains(
                MazeUtilities::positionAfterMovingForward(tile, direction)
            )) {
                continue;
            }
            if (!maze.at(tile.first).at(tile.second).value(direction)) {
                numberOfEntrances += 1;
            }
        }
    }
    return numberOfEntrances == 1;
}

bool MazeChecker::hasHollowCenter(const BasicMaze& maze) {
    const auto& centerPositions =
        MazeUtilities::getCenterPositions(maze.size(), maze.at(0).size()); 
    for (const auto& tile : centerPositions) {
        for (const auto& other : centerPositions) {
            for (const auto& direction : DIRECTIONS()) {
                if (
                    MazeUtilities::positionAfterMovingForward(tile, direction)
                    != other
                ) {
                    continue;
                }
                if (maze.at(tile.first).at(tile.second).value(direction)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool MazeChecker::hasWallAttachedToEachNonCenterPost(const BasicMaze& maze) {
    const auto& centerPositions =
        MazeUtilities::getCenterPositions(maze.size(), maze.at(0).size());
    for (int x = 0; x < maze.size() - 1; x += 1) {
        for (int y = 0; y < maze.at(x).size() - 1; y += 1) {
            // There is a wall attached
            if (
                maze.at(x).at(y).value(Direction::NORTH) ||
                maze.at(x).at(y).value(Direction::EAST) ||
                maze.at(x + 1).at(y + 1).value(Direction::SOUTH) ||
                maze.at(x + 1).at(y + 1).value(Direction::WEST)
            ) {
                continue;
            }
            // It is the center post
            QPair<int, int> position = {x, y};
            if (
                centerPositions.size() == 4 &&
                position == MazeUtilities::getMinPosition(centerPositions)
            ) {
                continue;
            }
            return false; 
        }
    }
    return true;
}

bool MazeChecker::isUnsolvableByWallFollower(const BasicMaze& maze) {
    const auto& centerPositions =
        MazeUtilities::getCenterPositions(maze.size(), maze.at(0).size());
    QSet<QPair<int, int>> reachable;
    QPair<int, int> start = {0, 0};
    QPair<int, int> position = start;
    Direction direction = Direction::NORTH;
    do {
        reachable.insert(position);
        Direction oldDirection = direction;
        Direction newDirection = DIRECTION_ROTATE_RIGHT().value(direction);
        if (!maze.at(position.first).at(position.second).value(newDirection)) {
            direction = newDirection;
        }
        while (maze.at(position.first).at(position.second).value(direction)) {
            direction = DIRECTION_ROTATE_LEFT().value(direction);
            if (direction == oldDirection) {
                // We're surrounded by walls
                return !centerPositions.contains(position);
            }
        }
        position =
            MazeUtilities::positionAfterMovingForward(position, direction);
    }
    while (position != start);
    for (const auto& tile : centerPositions) {
        if (reachable.contains(tile)) {
            return false;
        }
    }
    return true;
}

} // namespace mms
