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
    if (P()->useMazeFile()) {
        std::string mazeFilePath = SimUtilities::getProjectDirectory() + P()->mazeDirectory() + P()->mazeFile();
        if (!initializeViaMazeFile(mazeFilePath)) {
            SimUtilities::print("Error: Unable to initialize maze from file \"" + mazeFilePath + "\".");
            SimUtilities::quit();
        }
    }
    else {
        if (!initializeViaMazeAlgorithm()) {
            SimUtilities::print("Error: Unable to initialize maze via maze generation algorithm \""
                + P()->mazeAlgorithm() + "\".");
            SimUtilities::quit();
        }
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

bool Maze::initializeViaMazeFile(const std::string& mazeFilePath) {

    // Then, check to see if the file is in the correct format
    if (!MazeFileUtilities::isMazeFile(mazeFilePath)) {
        return false;
    }

    // If so, load the data into a maze
    std::vector<std::vector<BasicTile>> basicMaze = MazeFileUtilities::loadMaze(mazeFilePath);

    // Validate the maze. If we fail, bail.
    if (!validateBasicMaze(basicMaze)) {
        return false;
    }

    // Load the maze given by mazeFilePath
    initializeFromBasicMaze(basicMaze);

    return true;
}

bool Maze::initializeViaMazeAlgorithm() {

    // Check the parameters file for maze generation algo
    MazeAlgorithms mazeAlgorithms;
    std::map<std::string, IMazeAlgorithm*> algorithms = mazeAlgorithms.getAlgorithms();
    if (algorithms.find(sim::P()->mazeAlgorithm()) == algorithms.end()) {
        sim::SimUtilities::print("Error: \"" + sim::P()->mazeAlgorithm() + "\" is not a valid maze algorithm.");
        return false;
    }

    // Call on the maze generation algorithm to generate the maze
    std::vector<std::vector<BasicTile>> basicMaze = getBlankBasicMaze(P()->generatedMazeWidth(), P()->generatedMazeHeight());
    MazeInterface mazeInterface(&basicMaze);
    algorithms.at(sim::P()->mazeAlgorithm())->generate(
        P()->generatedMazeWidth(), P()->generatedMazeHeight(), &mazeInterface);

    // Validate the maze. If we fail, bail.
    if (!validateBasicMaze(basicMaze)) {
        return false;
    }

    // Load the maze given by the maze generation algorithm
    initializeFromBasicMaze(basicMaze);

    // TODO: MACK - can this fail???
    // Optionally save the maze
    if (P()->saveGeneratedMaze()) {
        MazeFileUtilities::saveMaze(extractBasicMaze(),
            SimUtilities::getProjectDirectory() + P()->mazeDirectory() + "auto_generated_maze.maz");
    }

    return true;
}

bool Maze::validateBasicMaze(const std::vector<std::vector<BasicTile>>& basicMaze) {

    // Check to see if it's a valid maze
    if (!MazeChecker::validMaze(basicMaze)) {
        SimUtilities::print("Error: Failed maze validation.");
        return false;
    }

    // Then, check and enforce official maze rules
    if (P()->enforceOfficialMazeRules() && !MazeChecker::officialMaze(basicMaze)) {
        SimUtilities::print("Error: Failed official maze validation.");
        return false;
    }

    return true;
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

} // namespace sim
