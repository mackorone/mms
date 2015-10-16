#include "MazeChecker.h"

#include "Assert.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

bool MazeChecker::validMaze(const std::vector<std::vector<BasicTile>>& maze) {

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
    ASSERT(isNonempty(maze));
    for (int i = 0; i < maze.size() - 1; i += 1) {
        if (maze.at(i).size() != maze.at(i + 1).size()) {
            return false;
        }
    }
    return true;
}

bool MazeChecker::isEnclosed(const std::vector<std::vector<BasicTile>>& maze) {
    ASSERT(isRectangular(maze));
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            if (1 < x && x < maze.size() - 1 && 1 < y && y < maze.at(x).size() - 1) {
                continue;
            }
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
    ASSERT(isEnclosed(maze));
    // TODO: upforgrabs
    // Implement this method so that it returns true if the walls of the maze
    // are consistent, and false otherwise.
    return true;
}

bool MazeChecker::officialMaze(const std::vector<std::vector<BasicTile>>& maze) {

    ASSERT(validMaze(maze));

    if (!isSquare(maze)) {
        L()->warn("The maze is not square.");
        return false;
    }

    if (!hasPathToCenter(maze)) {
        L()->warn("The maze does not have a path to the center.");
        return false;
    }

    if (!hasHollowCenter(maze)) {
        L()->warn("The maze does not have a hollow center.");
        return false;
    }

    if (!hasOneEntranceToCenter(maze)) {
        L()->warn("The center of the maze has more than one entrance.");
        return false;
    }

    if (!hasWallAttachedToEachPost(maze)) {
        L()->warn("There is at least one non-center post with no walls connected to it.");
        return false;
    }
    
    if (!hasThreeStartingWalls(maze)) {
        L()->warn("There are not exactly three starting walls.");
        return false;
    }

    if (!isUnsolvableByWallFollower(maze)) {
        L()->warn("The maze is solvable by a maze-following robot.");
        return false;
    }

    if (!hasNoInaccesibleLocations(maze)) {
        L()->warn("There are inaccessible locations in the maze.");
        return false;
    }

    return true;
}

bool MazeChecker::isSquare(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if the maze is square, and
    // false otherwise.
    return true;
}

bool MazeChecker::hasPathToCenter(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if there is a path to the
    // center of the maze, and false otherwise.
    return false;
}

bool MazeChecker::hasHollowCenter(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if the maze has no walls
    // in the center tiles, and false otherwise.
    return false;
}

bool MazeChecker::hasOneEntranceToCenter(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if there is exactly one
    // entrance into the center of the maze, and false otherwise.
    return false;
}

bool MazeChecker::hasWallAttachedToEachPost(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if there is at least one
    // wall attached to every post (except for the center post), and false
    // otherwise.
    return false;
}

bool MazeChecker::hasThreeStartingWalls(const std::vector<std::vector<BasicTile>>& maze) {
    // Assumptions:
    // 1) The maze is nonempty
    // 2) The mouse always starts in the lower-left corner
    auto walls = maze.at(0).at(0).walls;
    return walls.at(Direction::NORTH) != walls.at(Direction::EAST);
}

bool MazeChecker::isUnsolvableByWallFollower(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if the maze is unsolvable
    // by a wall-following robot, and false otherwise.
    return false;
}

bool MazeChecker::hasNoInaccesibleLocations(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: upforgrabs
    // Implement this method so that it returns true if there are no
    // inaccessible locations in the maze, and false otherwise.
    return false;
}

} // namespace sim
