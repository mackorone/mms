#include "Maze.h"

#include <vector>

#include "Assert.h"
#include "MazeChecker.h"
#include "MazeFileUtilities.h"
#include "MazeGenerator.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Tile.h"

namespace sim {

Maze::Maze() {
    // We're relying on short-circuit evaluating here
    if (!P()->useMazeFile() || !initializeViaMazeFile()) {
        initializeViaMazeGenerator();
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

bool Maze::initializeViaMazeFile() {

    // First, get the maze file path
    std::string mazeFilePath = SimUtilities::getProjectDirectory() + P()->mazeDirectory() + P()->mazeFile();

    // Then, check to see if the file is in the correct format
    if (!MazeFileUtilities::isMazeFile(mazeFilePath)) {
        SimUtilities::print("Error: \"" + mazeFilePath + "\" does not match the maze file format.");
        return false;
    }

    // If so, load the data into a maze
    std::vector<std::vector<BasicTile>> maze = MazeFileUtilities::loadMaze(mazeFilePath);

    // Then, check to see if it's a valid maze
    if (!MazeChecker::validMaze(maze)) {
        SimUtilities::print("Error: \"" + mazeFilePath + "\" failed maze validation.");
        return false;
    }

    // Then, check and enforce official maze rules
    if (P()->enforceOfficialMazeRules() && !MazeChecker::officialMaze(maze)) {
        SimUtilities::print("Error: \"" + mazeFilePath + "\" failed official maze validation.");
        return false;
    }

    // Load the maze given by mazeFilePath
    initializeMaze(maze);

    return true;
}

void Maze::initializeViaMazeGenerator() {

    // Check the parameters file for maze generation algo
    std::string mazeAlgo(P()->mazeGenerationAlgo());
    if (0 == MAZE_ALGOS.count(mazeAlgo)) {
        SimUtilities::print("Error: \"" + mazeAlgo + "\" is not a valid maze generation algorithm.");
        SimUtilities::quit();
    }

    // Load the maze given by the maze generation algorithm
    initializeMaze(MazeGenerator::generateMaze(MAZE_ALGOS.find(mazeAlgo)->second));

    // Optionally save the maze
    if (P()->saveGeneratedMaze()) {
        MazeFileUtilities::saveMaze(extractMaze(), SimUtilities::getProjectDirectory() + P()->mazeDirectory() + "auto_generated_maze.maz");
    }
}

void Maze::initializeMaze(const std::vector<std::vector<BasicTile>>& maze) {
    for (int x = 0; x < maze.size(); x += 1) {
        std::vector<Tile> column;
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            Tile tile;
            tile.setPos(x, y);
            for (Direction direction : DIRECTIONS) {
                tile.setWall(direction, maze.at(x).at(y).walls.at(direction));
            }
            tile.initPolygons();
            column.push_back(tile);
        }
        m_maze.push_back(column);
    }
}

std::vector<std::vector<BasicTile>> Maze::extractMaze() const {
    std::vector<std::vector<BasicTile>> maze;
    for (int x = 0; x < m_maze.size(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < m_maze.at(x).size(); y += 1) {
            BasicTile tile;
            tile.x = x;
            tile.y = y;
            for (Direction direction : DIRECTIONS) {
                tile.walls[direction] = getTile(x, y)->isWall(direction);
            }
            column.push_back(tile);
        }
        maze.push_back(column);
    }
    return maze;
}

} // namespace sim
