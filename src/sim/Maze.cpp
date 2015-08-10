#include "Maze.h"

#include <vector>

#include "../maze/IMazeAlgorithm.h"
#include "../maze/MazeAlgorithms.h"
#include "Assert.h"
#include "MazeChecker.h"
#include "MazeFileUtilities.h"
#include "MazeInterface.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Tile.h"

namespace sim {

Maze::Maze() {
    
    std::vector<std::vector<BasicTile>> basicMaze;

    if (P()->useMazeFile()) {

        std::string mazeFilePath = SimUtilities::getProjectDirectory() + P()->mazeDirectory() + P()->mazeFile();
        if (!MazeFileUtilities::isMazeFile(mazeFilePath)) {
            SimUtilities::print("Error: Unable to initialize maze from file \"" + mazeFilePath + "\".");
            SimUtilities::quit();
        }

        basicMaze = MazeFileUtilities::loadMaze(mazeFilePath);
    }
    else {

        MazeAlgorithms mazeAlgorithms;
        std::map<std::string, IMazeAlgorithm*> algorithms = mazeAlgorithms.getAlgorithms();
        if (!SimUtilities::mapContains(algorithms, sim::P()->mazeAlgorithm())) {
            SimUtilities::print("Error: \"" + sim::P()->mazeAlgorithm() + "\" is not a valid maze algorithm.");
            SimUtilities::quit();
        }

        basicMaze = getBlankBasicMaze(P()->generatedMazeWidth(), P()->generatedMazeHeight());
        MazeInterface mazeInterface(&basicMaze);
        algorithms.at(sim::P()->mazeAlgorithm())->generate(
            P()->generatedMazeWidth(), P()->generatedMazeHeight(), &mazeInterface);
    }

    // Check to see if it's a valid maze
    if (!MazeChecker::validMaze(basicMaze)) {
        SimUtilities::print("Error: Failed maze validation.");
        SimUtilities::quit();
    }

    // Mirror and rotate the maze
    if (P()->mazeMirrored()) {
        basicMaze = mirrorAcrossVertical(basicMaze);
    }
    for (int i = 0; i < P()->mazeRotations() % 4; i += 1) {
        basicMaze = rotateCounterClockwise(basicMaze);
    }

    // Then, check and enforce official maze rules
    if (P()->enforceOfficialMazeRules() && !MazeChecker::officialMaze(basicMaze)) {
        SimUtilities::print("Error: Failed official maze validation.");
        SimUtilities::quit();
    }

    // Load the maze given by the maze generation algorithm
    initializeFromBasicMaze(basicMaze);

    // Optionally save the maze
    if (P()->useMazeFile() && P()->saveGeneratedMaze()) {
        MazeFileUtilities::saveMaze(extractBasicMaze(),
            SimUtilities::getProjectDirectory() + P()->mazeDirectory() + "auto_generated_maze.maz");
    }
}

int Maze::getWidth() const {
    return m_maze.size();
}

int Maze::getHeight() const {
    return (m_maze.size() > 0 ? m_maze.at(0).size() : 0);
}

Tile* Maze::getTile(int x, int y) {
    ASSERT(0 <= x && x < getWidth() && 0 <= y && y < getHeight());
    return &m_maze.at(x).at(y);
}

const Tile* Maze::getTile(int x, int y) const {
    ASSERT(0 <= x && x < getWidth() && 0 <= y && y < getHeight());
    return &m_maze.at(x).at(y);
}

void Maze::initializeFromBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze) {
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
        m_maze.push_back(column);
    }
}

std::vector<std::vector<BasicTile>> Maze::extractBasicMaze() const {
    std::vector<std::vector<BasicTile>> basicMaze;
    for (int x = 0; x < m_maze.size(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < m_maze.at(x).size(); y += 1) {
            BasicTile tile;
            tile.x = x;
            tile.y = y;
            for (Direction direction : DIRECTIONS) {
                tile.walls.insert(std::make_pair(direction, getTile(x, y)->isWall(direction)));
            }
            column.push_back(tile);
        }
        basicMaze.push_back(column);
    }
    return basicMaze;
}

std::vector<std::vector<BasicTile>> Maze::getBlankBasicMaze(int mazeWidth, int mazeHeight) const {
    std::vector<std::vector<BasicTile>> blankMaze;
    for (int x = 0; x < mazeWidth; x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < mazeHeight; y += 1) {
            BasicTile tile;
            tile.x = x;
            tile.y = y;
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
            tile.x = x;
            tile.y = y;
            tile.walls.insert(std::make_pair(NORTH, basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(NORTH)));
            tile.walls.insert(std::make_pair(EAST, basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(WEST)));
            tile.walls.insert(std::make_pair(SOUTH, basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(SOUTH)));
            tile.walls.insert(std::make_pair(WEST, basicMaze.at(basicMaze.size() - 1 - x).at(y).walls.at(EAST)));
            column.push_back(tile);
        }
        mirrored.push_back(column);
    }
    return mirrored; 
}

std::vector<std::vector<BasicTile>> Maze::rotateCounterClockwise(const std::vector<std::vector<BasicTile>>& basicMaze) {

    // We can't rotate a non-rectangular maze
    ASSERT(MazeChecker::isRectangular(basicMaze));

    std::vector<std::vector<BasicTile>> rotated;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        std::vector<BasicTile> row;
        for (int y = basicMaze.at(x).size() - 1; y >= 0; y -= 1) {
            BasicTile tile;
            tile.x = basicMaze.at(x).size() - 1 - y;
            tile.y = x;
            tile.walls.insert(std::make_pair(NORTH, basicMaze.at(x).at(y).walls.at(EAST)));
            tile.walls.insert(std::make_pair(EAST, basicMaze.at(x).at(y).walls.at(SOUTH)));
            tile.walls.insert(std::make_pair(SOUTH, basicMaze.at(x).at(y).walls.at(WEST)));
            tile.walls.insert(std::make_pair(WEST, basicMaze.at(x).at(y).walls.at(NORTH)));
            if (rotated.size() <= tile.x) {
                rotated.push_back({tile});
            }
            else {
                rotated.at(tile.x).push_back(tile);
            }
        }
    }

    return rotated;
}

} // namespace sim
