#include "MazeChecker.h"

#include <functional>

#include "Assert.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

QPair<bool, QVector<QString>> MazeChecker::isDrawableMaze(const BasicMaze& maze) {
    QVector<QString> errors;
    errors += isNonempty(maze);
    errors += isRectangular(maze);
    return {errors.empty(), errors};
}

QPair<bool, QVector<QString>> MazeChecker::isValidMaze(const BasicMaze& maze) {
    SIM_ASSERT_TR(isDrawableMaze(maze).first);
    QVector<QString> errors;
    errors += isEnclosed(maze);
    errors += hasConsistentWalls(maze);
    return {errors.empty(), errors};
}

QPair<bool, QVector<QString>> MazeChecker::isOfficialMaze(const BasicMaze& maze) {
    SIM_ASSERT_TR(isValidMaze(maze).first);
    QVector<QString> errors;
    errors += hasNoInaccessibleLocations(maze);
    errors += hasThreeStartingWalls(maze);
    errors += hasOneEntranceToCenter(maze);
    errors += hasHollowCenter(maze);
    errors += hasWallAttachedToEachNonCenterPost(maze);
    errors += isUnsolvableByWallFollower(maze);
    return {errors.empty(), errors};
}

QSet<QPair<int, int>> MazeChecker::getCenterTiles(int width, int height) {
    QSet<QPair<int, int>> centerTiles;
            centerTiles.insert({(width - 1) / 2, (height - 1) / 2});
    if (width % 2 == 0) {
            centerTiles.insert({ width      / 2, (height - 1) / 2});
        if (height % 2 == 0) {
            centerTiles.insert({(width - 1) / 2,  height      / 2});
            centerTiles.insert({ width      / 2,  height      / 2});
        }
    }
    else if (height % 2 == 0) {
            centerTiles.insert({(width - 1) / 2,  height      / 2});
    }
    return centerTiles; 
}

QVector<QString> MazeChecker::isNonempty(const BasicMaze& maze) {
    // TODO: MACK - test this
    for (int i = 0; i < maze.size(); i += 1) {
        if (0 < maze.at(i).size()) {
            return {};
        }
    }
    return {"The maze is empty."};
}

QVector<QString> MazeChecker::isRectangular(const BasicMaze& maze) {
    // TODO: MACK - test this
    for (int i = 0; i < maze.size() - 1; i += 1) {
        if (maze.at(i).size() != maze.at(i + 1).size()) {
            return {QString(
                "The maze is not rectangular: column %1 has %2 tiles but column"
                " %3 has %4 tiles.")
                .arg(i)
                .arg(maze.at(i).size())
                .arg(i + 1)
                .arg(maze.at(i + 1).size())
            };
        }
    }
    return {};
}

QVector<QString> MazeChecker::isEnclosed(const BasicMaze& maze) {
    // TODO: MACK - test this
    static QVector<QPair<std::function<bool(int, int)>, Direction>> requirements {
        {[&maze](int x, int y){return x ==               0;}, Direction::WEST},
        {[&maze](int x, int y){return y ==               0;}, Direction::SOUTH},
        {[&maze](int x, int y){return x == maze.size() - 1;}, Direction::EAST},
        {[&maze](int x, int y){return y == maze.size() - 1;}, Direction::NORTH},
    };
    QVector<QString> errors;
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            for (auto pair : requirements) {
                if (pair.first(x, y) && !maze.at(x).at(y).value(pair.second)) {
                    errors.push_back(QString(
                        "The maze is not enclosed by walls: tile (%1, %2) has"
                        " no %3 wall.")
                        .arg(x)
                        .arg(y)
                        .arg(DIRECTION_TO_STRING.value(pair.second))
                    );
                }
            }
        }
    }
    return errors;
}

QVector<QString> MazeChecker::hasConsistentWalls(const BasicMaze& maze) {
    // TODO: MACK - test this
    static QVector<QPair<std::function<bool(int, int)>, Direction>> requirements {
        {[&maze](int x, int y){return 0 <               x;}, Direction::WEST},
        {[&maze](int x, int y){return 0 <               y;}, Direction::SOUTH},
        {[&maze](int x, int y){return x < maze.size() - 1;}, Direction::EAST},
        {[&maze](int x, int y){return y < maze.size() - 1;}, Direction::NORTH},
    };
    QVector<QString> errors;
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            for (auto pair : requirements) {
                if (pair.first(x, y) && maze.at(x).at(y).value(pair.second)) {
                    QPair<int, int> other = positionAfterMovingForward({x, y}, pair.second);
                    Direction opposite = DIRECTION_OPPOSITE.value(pair.second);
                    if (!maze.at(other.first).at(other.second).value(opposite)) {
                        errors.push_back(QString(
                            "The maze does not have consistent walls: tile"
                            " (%1, %2) has a %3 wall but tile (%4, %5) has no"
                            " %6 wall")
                            .arg(x)
                            .arg(y)
                            .arg(DIRECTION_TO_STRING.value(pair.second))
                            .arg(other.first)
                            .arg(other.second)
                            .arg(DIRECTION_TO_STRING.value(opposite))
                        );
                    }
                }
            }
        }
    }
    return errors;
}

QVector<QString> MazeChecker::hasNoInaccessibleLocations(const BasicMaze& maze) {

    // TODO: MACK - test this
    /*

    QSet<QPair<int, int>> discovered;
    QQueue<QPair<int, int>> queue;

    for (QPair<int, int> tile : getCenterTiles(maze.size(), maze.at(0).size())) {
        discovered.insert(tile);
        queue.push(tile);
    }

    // BFS search
    while (!queue.isEmpty()) {
        QPair<int, int> tile = queue.dequeue();
        for (Direction direction : DIRECTIONS) {
            if (maze.at(tile.first).at(tile.second).value(direction)) {
                continue;
            }
            QPair<int, int> neighbor = positionAfterMovingForward(tile, direction);
            if (!discovered.contains(neighbor)) {
                discovered.insert(neighbor);
                queue.enqueue(neighbor);
            }
        }
    }
    
    QVector<QPair<int, int>> inaccessibleTiles;
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            if (!discovered.contains({x, y})) {
                inaccessibleTiles.insert({x, y});
            }
        }
    }

    if (!inaccessibleTiles.isEmpty()) {
        return {QString(
            "There maze has inaccessible tiles: %1.");
            .arg(inaccessibleTiles)
        };
    }
    */

    return {};
}

QVector<QString> MazeChecker::hasThreeStartingWalls(const BasicMaze& maze) {
    /*
        L()->warn("There are not exactly three starting walls.");
    QMap<Direction, bool> walls = maze.at(0).at(0);
    return walls.at(Direction::NORTH) != walls.at(Direction::EAST);
    */
    return {};
}

QVector<QString> MazeChecker::hasOneEntranceToCenter(const BasicMaze& maze) {
    // TODO: MACK - fix this to work with QVector
    /*
        L()->warn("The center of the maze has more than one entrance.");
    QVector<std::pair<int, int>> centerTiles = getCenterTiles(maze.size(), maze.at(0).size()); 
    int numberOfEntrances = 0;
    for (std::pair<int, int> tile : centerTiles) {
        for (Direction direction : DIRECTIONS) {
            if (ContainerUtilities::vectorContains(centerTiles, positionAfterMovingForward(tile, direction))) {
                continue;
            }
            if (!maze.at(tile.first).at(tile.second).walls.at(direction)) {
                numberOfEntrances += 1;
            }
        }
    }
    return numberOfEntrances == 1;
    */
    return {};
}

QVector<QString> MazeChecker::hasHollowCenter(const BasicMaze& maze) {
    /*
        L()->warn("The maze does not have a hollow center.");

    QVector<std::pair<int, int>> centerTiles = getCenterTiles(maze.size(), maze.at(0).size()); 
    for (std::pair<int, int> tile : centerTiles) {
        for (std::pair<int, int> otherTile : centerTiles) {
            for (Direction direction : DIRECTIONS) {
                if (positionAfterMovingForward(tile, direction) != otherTile) {
                    continue;
                }
                if (maze.at(tile.first).at(tile.second).walls.at(direction)) {
                    return false;
                }
            }
        }
    }
    return true;
    */
    return {};
}

QVector<QString> MazeChecker::hasWallAttachedToEachNonCenterPost(const BasicMaze& maze) {
    // TODO: MACK - fix this to work with QVector
    /*
        L()->warn("There is at least one non-center post with no walls connected to it.");
    QVector<std::pair<int, int>> centerTiles = getCenterTiles(maze.size(), maze.at(0).size());
    auto upperRightPostIsCenterPost = [&](int x, int y) {
        return centerTiles.size() == 4 && std::make_pair(x, y) == SimUtilities::min(centerTiles);
    };
    for (int x = 0; x < maze.size() - 1; x += 1) {
        for (int y = 0; y < maze.at(x).size() - 1; y += 1) {
            QVector<bool> conditions {
                maze.at(x).at(y).walls.at(Direction::NORTH),
                maze.at(x).at(y).walls.at(Direction::EAST),
                maze.at(x + 1).at(y + 1).walls.at(Direction::SOUTH),
                maze.at(x + 1).at(y + 1).walls.at(Direction::WEST),
                upperRightPostIsCenterPost(x, y)
            };
            if (std::none_of(conditions.begin(), conditions.end(), [](bool b){return b;})) {
                return false; 
            }
        }
    }
    return true;
    */
    return {};
}

QVector<QString> MazeChecker::isUnsolvableByWallFollower(const BasicMaze& maze) {

    /*
        L()->warn("The maze is solvable by a maze-following robot.");
    std::set<std::pair<int, int>> reachableByWallFollower;

    std::pair<int, int> position = std::make_pair(0, 0);
    Direction direction = Direction::NORTH;

    while (!ContainerUtilities::setContains(reachableByWallFollower, position)) {
        Direction oldDirection = direction;
        Direction newDirection = DIRECTION_ROTATE_RIGHT.at(direction);
        if (!maze.at(position.first).at(position.second).walls.at(newDirection)) {
            direction = newDirection;
        }
        while (maze.at(position.first).at(position.second).walls.at(direction)) {
            direction = DIRECTION_ROTATE_LEFT.at(direction);
            if (direction == oldDirection) {
                break;
            }
        }
        position = positionAfterMovingForward(position, direction);
        reachableByWallFollower.insert(position);
    }
    
    for (std::pair<int, int> tile : getCenterTiles(maze.size(), maze.at(0).size())) {
        if (ContainerUtilities::setContains(reachableByWallFollower, tile)) {
            return false;
        }
    }

    return true;
    */
    return {};
}

QPair<int, int> MazeChecker::positionAfterMovingForward(
        QPair<int, int> position,
        Direction direction) {
    return {
        // X coordinate
        (direction == Direction::EAST ?  position.first + 1 :
        (direction == Direction::WEST ?  position.first - 1 :
        position.first)),
        // Y coordinate
        (direction == Direction::NORTH ?  position.second + 1 :
        (direction == Direction::SOUTH ?  position.second - 1 :
        position.second))
    };
}

} // namespace sim
