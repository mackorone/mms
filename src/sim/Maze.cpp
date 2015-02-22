#include "Maze.h"

#include <cstdlib>
#include <math.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <time.h>
#include <queue>
#include <vector>
#include <stack>

#include "MazeFileUtilities.h"
#include "MazeGenerator.h"
#include "Param.h"
#include "Tile.h"
#include "SimUtilities.h"

namespace sim {

Maze::Maze() {
    // We're relying on short-circuit evaluating here
    if (!P()->useMazeFile() || !initializeViaMazeFile()) {
        initializeViaMazeGenerator();
    }
}

int Maze::getWidth() {
    return m_maze.size();
}

int Maze::getHeight() {
    return (m_maze.size() > 0 ? m_maze.at(0).size() : 0);
}

Tile* Maze::getTile(int x, int y) {
    return &m_maze.at(x).at(y);
}

bool Maze::initializeViaMazeFile() {

    // First, get the maze file path
    std::string mazeFilePath = getProjectDirectory() + P()->mazeDirectory() + P()->mazeFile();

    // Then, check to see if it's a valid
    if (!validMaze(mazeFilePath)) {
        print("Error: \"" + mazeFilePath + "\" failed maze validation");
        return false;
    }

    // Then, check and enforce official maze rules
    if (P()->enforceOfficialMazeRules() && !officialMaze(mazeFilePath)) {
        print("Error: \"" + mazeFilePath + "\" failed official maze validation");
        return false;
    }
    
    // Load the maze given by mazeFilePath
    initializeMaze(loadMaze(mazeFilePath));

    return true;
}

void Maze::initializeViaMazeGenerator() {

    // Check the parameters file for random algo
    std::string mazeAlgo(P()->randomMazeAlgo());
    if (0 == MAZE_ALGOS.count(mazeAlgo)) {
        print("Error: \"" + mazeAlgo + "\" is not a valid maze algorithm");
        mazeAlgo = DEFAULT_MAZE_ALGO;
    }

    // Load the maze given by the maze generation algorithm
    initializeMaze(MazeGenerator::generateMaze(MAZE_ALGOS.find(mazeAlgo)->second));
    
    // Optionally save the maze
    if (P()->saveRandomMaze()) {
        saveMaze(extractMaze(), getProjectDirectory() + P()->mazeDirectory() + "auto_generated_maze.maz");
    }
}

void Maze::initializeMaze(std::vector<std::vector<BasicTile>> maze) {
    for (int x = 0; x < maze.size(); x += 1) {
        std::vector<Tile> column;
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            Tile tile;
            tile.setPos(x, y);
            for (Direction direction : DIRECTIONS) {
                tile.setWall(direction, maze.at(x).at(y).walls[direction]);
            }
            column.push_back(tile);
        }
        m_maze.push_back(column);
    }
}

std::vector<std::vector<BasicTile>> Maze::extractMaze() {
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
