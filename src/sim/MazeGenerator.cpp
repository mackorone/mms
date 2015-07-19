#include "MazeGenerator.h"

#include "Param.h"
#include "SimUtilities.h"
#include "TomaszMazeGenerator.h"

namespace sim {

std::vector<std::vector<BasicTile>> MazeGenerator::generateMaze(Algorithm algo) {

    // Get the maze based on the choice of algorithm
    std::vector<std::vector<BasicTile>> maze;
    switch (algo) {
        case TOMASZ:
            maze = tomasz();
            break;
        case RANDOM:
            maze = random();
            break;
    }
    return maze;
}

std::vector<std::vector<BasicTile>> MazeGenerator::random() {

    // Create the maze that will be returned
    std::vector<std::vector<BasicTile>> maze;
    for (int x = 0; x < P()->generatedMazeWidth(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < P()->generatedMazeHeight(); y += 1) {
            BasicTile tile;
            tile.x = x;
            tile.y = y;
            column.push_back(tile);
        }
        maze.push_back(column);
    }

    // Percentage chance any one wall will exist
    float wallProb = 0.40;
    for (int x = 0; x < maze.size(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            for (Direction direction : DIRECTIONS) {
                bool wallExists = (SimUtilities::getRandom() <= wallProb);
                switch (direction) {
                    case NORTH:
                        if (y + 1 < P()->generatedMazeHeight()) {
                            maze.at(x).at(y).walls[NORTH] = wallExists;
                            maze.at(x).at(y + 1).walls[SOUTH] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[NORTH] = true;
                        }
                        break;
                    case EAST:
                        if (x + 1 < P()->generatedMazeWidth()) {
                            maze.at(x).at(y).walls[EAST] = wallExists;
                            maze.at(x + 1).at(y).walls[WEST] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[EAST] = true;
                        }
                        break;
                    case SOUTH:
                        if (y > 0) {
                            maze.at(x).at(y).walls[SOUTH] = wallExists;
                            maze.at(x).at(y - 1).walls[NORTH] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[SOUTH] = true;
                        }
                        break;
                    case WEST:
                        if (x > 0) {
                            maze.at(x).at(y).walls[WEST] = wallExists;
                            maze.at(x - 1).at(y).walls[EAST] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[WEST] = true;
                        }
                        break;
                }
            }
        }
    }
    return maze;
}

std::vector<std::vector<BasicTile>> MazeGenerator::tomasz() {
    TomaszMazeGenerator p;
    return p.convertToBasicMaze();
}

} // namespace sim
