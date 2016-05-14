#include "Maze.h"

#include <queue>
#include <set>
#include <vector>

#include "../maze/IMazeAlgorithm.h"
#include "../maze/MazeAlgorithms.h"
#include "Assert.h"
#include "Directory.h"
#include "Logging.h"
#include "MazeChecker.h"
#include "MazeFileType.h"
#include "MazeFileUtilities.h"
#include "MazeInterface.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Tile.h"

namespace sim {

Maze::Maze() {
    
    std::vector<std::vector<BasicTile>> basicMaze;

    if (P()->useMazeFile()) {
        std::string mazeFilePath = Directory::getResMazeDirectory() + P()->mazeFile();
        try {
            basicMaze = MazeFileUtilities::loadMaze(mazeFilePath);
        }
        catch (...) {
            std::string reason = (
                SimUtilities::isFile(mazeFilePath) ?
                "invalid format" : "file doesn't exist");
            L()->error(
                "Unable to initialize maze from file \"%v\": %v.",
                mazeFilePath,
                reason);
            SimUtilities::quit();
        }
    }
    else {
        if (!MazeAlgorithms::isMazeAlgorithm(P()->mazeAlgorithm())) {
            L()->error("\"%v\" is not a valid maze algorithm.", P()->mazeAlgorithm());
            SimUtilities::quit();
        }
        basicMaze = getBlankBasicMaze(P()->generatedMazeWidth(), P()->generatedMazeHeight());
        MazeInterface mazeInterface(&basicMaze);
        MazeAlgorithms::getMazeAlgorithm(P()->mazeAlgorithm())->generate(
            P()->generatedMazeWidth(), P()->generatedMazeHeight(), &mazeInterface);
    }

    // Check to see if it's a valid maze
    m_isValidMaze = MazeChecker::isValidMaze(basicMaze);
    if (!m_isValidMaze) {
        L()->warn("The maze failed validation. The mouse algorithm will not execute.");
    }

    // Optionally save the maze
    if (!P()->useMazeFile() && P()->saveGeneratedMaze()) {
        MazeFileType type = STRING_TO_MAZE_FILE_TYPE.at(P()->generatedMazeType());
        std::string mazeFilePath = Directory::getResMazeDirectory() +
            P()->generatedMazeFile() + MAZE_FILE_TYPE_TO_SUFFIX.at(type);
        bool success = MazeFileUtilities::saveMaze(basicMaze, mazeFilePath, type);
        if (success) {
            L()->info("Maze saved to \"%v\".", mazeFilePath);
        }
        else {
            L()->warn("Unable to save maze to \"%v\".", mazeFilePath);
        }
    }

    // Mirror and rotate the maze
    if (P()->mazeMirrored()) {
        basicMaze = mirrorAcrossVertical(basicMaze);
        L()->info("Mirroring the maze across the vertical.");
    }
    for (int i = 0; i < P()->mazeRotations(); i += 1) {
        basicMaze = rotateCounterClockwise(basicMaze);
        L()->info("Rotating the maze counter-clockwise (%vx).", i + 1);
    }

    // Then, store whether or not the maze is an official maze
    m_isOfficialMaze = m_isValidMaze && MazeChecker::isOfficialMaze(basicMaze);
    if (m_isValidMaze && !m_isOfficialMaze) {
        L()->warn("The maze did not pass the \"is official maze\" tests.");
    }

    // Load the maze given by the maze generation algorithm
    m_maze = initializeFromBasicMaze(basicMaze);
}

int Maze::getWidth() const {
    return m_maze.size();
}

int Maze::getHeight() const {
    return (m_maze.size() > 0 ? m_maze.at(0).size() : 0);
}

bool Maze::withinMaze(int x, int y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

Tile* Maze::getTile(int x, int y) {
    SIM_ASSERT_TR(withinMaze(x, y));
    return &m_maze.at(x).at(y);
}

const Tile* Maze::getTile(int x, int y) const {
    SIM_ASSERT_TR(withinMaze(x, y));
    return &m_maze.at(x).at(y);
}

bool Maze::isValidMaze() const {
    return m_isOfficialMaze;
}

bool Maze::isOfficialMaze() const {
    return m_isOfficialMaze;
}

bool Maze::isCenterTile(int x, int y) const {
    return ContainerUtilities::vectorContains(
        MazeChecker::getCenterTiles(getWidth(), getHeight()),
        std::make_pair(x, y)
    );
}

std::vector<std::vector<Tile>> Maze::initializeFromBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze) {
    std::vector<std::vector<Tile>> maze;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        std::vector<Tile> column;
        for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
            Tile tile;
            tile.setPos(x, y);
            for (Direction direction : DIRECTIONS) {
                tile.setWall(direction, basicMaze.at(x).at(y).walls.at(direction));
            }
            tile.initPolygons(basicMaze.size(), basicMaze.at(x).size());
            column.push_back(tile);
        }
        maze.push_back(column);
    }
    maze = setTileDistances(maze);
    return maze;
}

std::vector<std::vector<BasicTile>> Maze::getBlankBasicMaze(int mazeWidth, int mazeHeight) {
    std::vector<std::vector<BasicTile>> blankMaze;
    for (int x = 0; x < mazeWidth; x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < mazeHeight; y += 1) {
            BasicTile tile;
            for (Direction direction : DIRECTIONS) {
                tile.walls.insert(std::make_pair(direction, false));
            }
            column.push_back(tile);
        }
        blankMaze.push_back(column);
    }
    return blankMaze;
}

std::vector<std::vector<BasicTile>> Maze::mirrorAcrossVertical(const std::vector<std::vector<BasicTile>>& basicMaze) {
    std::vector<std::vector<BasicTile>> mirrored;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
            BasicTile tile;
            tile.walls.insert(std::make_pair(Direction::NORTH,
                basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(Direction::NORTH)));
            tile.walls.insert(std::make_pair(Direction::EAST,
                basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(Direction::WEST)));
            tile.walls.insert(std::make_pair(Direction::SOUTH,
                basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(Direction::SOUTH)));
            tile.walls.insert(std::make_pair(Direction::WEST,
                basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(Direction::EAST)));
            column.push_back(tile);
        }
        mirrored.push_back(column);
    }
    return mirrored; 
}

std::vector<std::vector<BasicTile>> Maze::rotateCounterClockwise(const std::vector<std::vector<BasicTile>>& basicMaze) {
    std::vector<std::vector<BasicTile>> rotated;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        std::vector<BasicTile> row;
        for (int y = basicMaze.at(x).size() - 1; y >= 0; y -= 1) {
            BasicTile tile;
            int rotatedTileX = basicMaze.at(x).size() - 1 - y;
            tile.walls.insert(std::make_pair(Direction::NORTH, basicMaze.at(x).at(y).walls.at(Direction::EAST)));
            tile.walls.insert(std::make_pair(Direction::EAST, basicMaze.at(x).at(y).walls.at(Direction::SOUTH)));
            tile.walls.insert(std::make_pair(Direction::SOUTH, basicMaze.at(x).at(y).walls.at(Direction::WEST)));
            tile.walls.insert(std::make_pair(Direction::WEST, basicMaze.at(x).at(y).walls.at(Direction::NORTH)));
            if (rotated.size() <= rotatedTileX) {
                rotated.push_back({tile});
            }
            else {
                rotated.at(rotatedTileX).push_back(tile);
            }
        }
    }
    return rotated;
}

std::vector<std::vector<Tile>> Maze::setTileDistances(std::vector<std::vector<Tile>> maze) {

    // The maze is guarenteed to be nonempty and rectangular
    int width = maze.size();
    int height = maze.at(0).size();

    // Helper lambda for retrieving and adjacent tile if one exists, nullptr if not
    auto getNeighbor = [&maze, &width, &height](int x, int y, Direction direction) {
        switch (direction) {
            case Direction::NORTH:
                return (y < height - 1 ? &maze.at(x).at(y + 1) : nullptr);
            case Direction::EAST:
                return (x < width - 1 ? &maze.at(x + 1).at(y) : nullptr);
            case Direction::SOUTH:
                return (0 < y ? &maze.at(x).at(y - 1) : nullptr);
            case Direction::WEST:
                return (0 < x ? &maze.at(x - 1).at(y) : nullptr);
        }
    };

    // Determine all of the center tiles
    std::vector<Tile*> centerTiles;
            centerTiles.push_back(&maze.at((width - 1) / 2).at((height - 1) / 2));
    if (width % 2 == 0) {
            centerTiles.push_back(&maze.at( width      / 2).at((height - 1) / 2));
        if (height % 2 == 0) {
            centerTiles.push_back(&maze.at((width - 1) / 2).at( height      / 2));
            centerTiles.push_back(&maze.at( width      / 2).at( height      / 2));
        }
    }
    else if (height % 2 == 0) {
            centerTiles.push_back(&maze.at((width - 1) / 2).at( height      / 2));
    }

    // The queue for the BFS
    std::queue<Tile*> discovered;

    // Set the distances of the center tiles and push them to the queue
    for (Tile* tile : centerTiles) {
        tile->setDistance(0); 
        discovered.push(tile);
    }

    // Now do a BFS
    while (!discovered.empty()){
        Tile* tile = discovered.front();
        discovered.pop(); // Removes the element
        for (Direction direction : DIRECTIONS) {
            if (!tile->isWall(direction)) {
                Tile* neighbor = getNeighbor(tile->getX(), tile->getY(), direction);
                if (neighbor != nullptr && neighbor->getDistance() == -1) {
                    neighbor->setDistance(tile->getDistance() + 1);
                    discovered.push(neighbor);
                }
            }
        }
    }

    return maze;
}

} // namespace sim
