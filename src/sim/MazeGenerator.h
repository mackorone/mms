#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

// A class that encapsulates the maze generation functions
class MazeGenerator {

public:
    // Different types of maze generation algorithms
    enum Algorithm { RANDOM = 0, TOMASZ = 1 };

    // Generate a maze based with a particular maze generation algorithm
    static std::vector<std::vector<BasicTile>> generateMaze(Algorithm algo);

private:

    // A private constructor to restrict creation of any MazeGenerator objects
    MazeGenerator();

    // Random maze generation algo
    static std::vector<std::vector<BasicTile>> random();

    // Tomasz's maze generation algo
    static std::vector<std::vector<BasicTile>> tomasz();

    /*
    // Randomly generate a maze
    void randomize();
    void tom_random();
    //Initialize starting Maze
    void initializeMaze();
    // Break down a wall
    void setWall(int x, int y, Direction direction, bool value);
    // TODO: Refactor this...
    void breakGradientWall();
    void addMinPeg();
    void surroundCenter();
    void pathIntoCenter();
    void hollowCenter();
    */
};

static const std::string DEFAULT_MAZE_ALGO = "TOMASZ";
static const std::map<std::string, MazeGenerator::Algorithm> MAZE_ALGOS = {
    {"TOMASZ", MazeGenerator::TOMASZ},
    {"RANDOM", MazeGenerator::RANDOM},
};

} // namespace sim
