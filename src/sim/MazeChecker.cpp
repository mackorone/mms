#include "MazeChecker.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

bool MazeChecker::isValidMaze(const std::vector<std::vector<BasicTile>>& maze) {

    if (!isNonempty(maze)) {
        L()->warn("The maze is empty.");
        return false;
    }

    if (!isRectangular(maze)) {
        L()->warn("The maze is not rectangular.");
        return false;
    }

    if (!isEnclosed(maze)) {
        L()->warn("The maze is not enclosed by walls.");
        return false;
    }

    if (!hasConsistentWalls(maze)) {
        L()->warn("The maze does not have consistent walls.");
        return false;
    }

    return true;
}

bool MazeChecker::isNonempty(const std::vector<std::vector<BasicTile>>& maze) {
    return (maze.size() == 0 ? false : 0 < maze.at(0).size());
}

bool MazeChecker::isRectangular(const std::vector<std::vector<BasicTile>>& maze) {
    for (int i = 0; i < maze.size() - 1; i += 1) {
        if (maze.at(i).size() != maze.at(i + 1).size()) {
            return false;
        }
    }
    return true;
}

bool MazeChecker::isEnclosed(const std::vector<std::vector<BasicTile>>& maze) {
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            if (x == 0 && !maze.at(x).at(y).walls.at(Direction::WEST)) {
                return false; 
            }
            if (y == 0 && !maze.at(x).at(y).walls.at(Direction::SOUTH)) {
                return false; 
            }
            if (x == maze.size() - 1 && !maze.at(x).at(y).walls.at(Direction::EAST)) {
                return false; 
            }
            if (y == maze.at(x).size() - 1 && !maze.at(x).at(y).walls.at(Direction::NORTH)) {
                return false; 
            }
        }
    }
    return true;
}

bool MazeChecker::hasConsistentWalls(const std::vector<std::vector<BasicTile>>& maze) {
    SIM_ASSERT_TR(isEnclosed(maze));
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            if (0 < x) {
                if (maze.at(x).at(y).walls.at(Direction::WEST) != maze.at(x - 1).at(y).walls.at(Direction::EAST)) {
                    return false;
                }
            }
            if (0 < y) {
                if (maze.at(x).at(y).walls.at(Direction::SOUTH) != maze.at(x).at(y - 1).walls.at(Direction::NORTH)) {
                    return false;
                }
            }
            if (x < maze.size() - 1) {
                if (maze.at(x).at(y).walls.at(Direction::EAST) != maze.at(x + 1).at(y).walls.at(Direction::WEST)) {
                    return false;
                }
            }
            if (y < maze.at(x).size() - 1) {
                if (maze.at(x).at(y).walls.at(Direction::NORTH) != maze.at(x).at(y + 1).walls.at(Direction::SOUTH)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool MazeChecker::isOfficialMaze(const std::vector<std::vector<BasicTile>>& maze) {

    SIM_ASSERT_TR(isValidMaze(maze));

    bool success = true;

    if (!hasNoInaccessibleLocations(maze)) {
        L()->warn("There are inaccessible locations in the maze.");
        success = false;
    }

    if (!hasHollowCenter(maze)) {
        L()->warn("The maze does not have a hollow center.");
        success = false;
    }

    if (!hasOneEntranceToCenter(maze)) {
        L()->warn("The center of the maze has more than one entrance.");
        success = false;
    }

    if (!hasWallAttachedToEachNonCenterPost(maze)) {
        L()->warn("There is at least one non-center post with no walls connected to it.");
        success = false;
    }
    
    if (!hasThreeStartingWalls(maze)) {
        L()->warn("There are not exactly three starting walls.");
        success = false;
    }

    if (!isUnsolvableByWallFollower(maze)) {
        L()->warn("The maze is solvable by a maze-following robot.");
        success = false;
    }

    return success;
}

std::vector<std::pair<int, int>> MazeChecker::getCenterTiles(int width, int height) {
    std::vector<std::pair<int, int>> centerTiles;
            centerTiles.push_back(std::make_pair((width - 1) / 2, (height - 1) / 2));
    if (width % 2 == 0) {
            centerTiles.push_back(std::make_pair( width      / 2, (height - 1) / 2));
        if (height % 2 == 0) {
            centerTiles.push_back(std::make_pair((width - 1) / 2,  height      / 2));
            centerTiles.push_back(std::make_pair( width      / 2,  height      / 2));
        }
    }
    else if (height % 2 == 0) {
            centerTiles.push_back(std::make_pair((width - 1) / 2,  height      / 2));
    }
    return centerTiles; 
}

bool MazeChecker::hasNoInaccessibleLocations(const std::vector<std::vector<BasicTile>>& maze) {

    std::set<std::pair<int, int>> discovered;
    std::queue<std::pair<int, int>> queue;

    for (std::pair<int, int> tile : getCenterTiles(maze.size(), maze.at(0).size())) {
        discovered.insert(tile);
        queue.push(tile);
    }

    while (!queue.empty()) {
        std::pair<int, int> tile = queue.front();
        queue.pop();
        for (Direction direction : DIRECTIONS) {
            if (maze.at(tile.first).at(tile.second).walls.at(direction)) {
                continue;
            }
            std::pair<int, int> neighbor = positionAfterMovingForward(tile, direction);
            if (!ContainerUtilities::setContains(discovered, neighbor)) {
                discovered.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }
    
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            if (!ContainerUtilities::setContains(discovered, std::make_pair(x, y))) {
                return false;
            }
        }
    }

    return true;
}

bool MazeChecker::hasThreeStartingWalls(const std::vector<std::vector<BasicTile>>& maze) {
    std::map<Direction, bool> walls = maze.at(0).at(0).walls;
    return walls.at(Direction::NORTH) != walls.at(Direction::EAST);
}

bool MazeChecker::hasOneEntranceToCenter(const std::vector<std::vector<BasicTile>>& maze) {
    std::vector<std::pair<int, int>> centerTiles = getCenterTiles(maze.size(), maze.at(0).size()); 
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
}

bool MazeChecker::hasHollowCenter(const std::vector<std::vector<BasicTile>>& maze) {
    std::vector<std::pair<int, int>> centerTiles = getCenterTiles(maze.size(), maze.at(0).size()); 
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
}

bool MazeChecker::hasWallAttachedToEachNonCenterPost(const std::vector<std::vector<BasicTile>>& maze) {
    std::vector<std::pair<int, int>> centerTiles = getCenterTiles(maze.size(), maze.at(0).size());
    auto upperRightPostIsCenterPost = [&](int x, int y) {
        return centerTiles.size() == 4 && std::make_pair(x, y) == SimUtilities::min(centerTiles);
    };
    for (int x = 0; x < maze.size() - 1; x += 1) {
        for (int y = 0; y < maze.at(x).size() - 1; y += 1) {
            std::vector<bool> conditions {
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
}

bool MazeChecker::isUnsolvableByWallFollower(const std::vector<std::vector<BasicTile>>& maze) {

    std::set<std::pair<int, int>> reachableByWallFollower;

    std::pair<int, int> position = std::make_pair(0, 0);
    Direction direction = Direction::NORTH;

    while (!ContainerUtilities::setContains(reachableByWallFollower, position)) {
        Direction oldDirection = direction;
        Direction newDirection = directionAfterRightTurn(direction);
        if (!maze.at(position.first).at(position.second).walls.at(newDirection)) {
            direction = newDirection;
        }
        while (maze.at(position.first).at(position.second).walls.at(direction)) {
            direction = directionAfterLeftTurn(direction);
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
}

Direction MazeChecker::directionAfterLeftTurn(Direction direction) {
    return DIRECTION_ROTATE_LEFT.at(direction);
}

Direction MazeChecker::directionAfterRightTurn(Direction direction) {
    return DIRECTION_ROTATE_RIGHT.at(direction);
}

std::pair<int, int> MazeChecker::positionAfterMovingForward(std::pair<int, int> position, Direction direction) {
    return {
        (direction == Direction::EAST ?
            position.first + 1 :
        (direction == Direction::WEST ?
            position.first - 1 :
        position.first)),
        (direction == Direction::NORTH ?
            position.second + 1 :
        (direction == Direction::SOUTH ?
            position.second - 1 :
        position.second))
    };
}

} // namespace sim
