#pragma once

#include <string>
#include <vector>

#include "BasicTile.h"
#include "Tile.h"

namespace sim {

class Maze {

public:
    Maze();
    int getWidth();
    int getHeight();
    Tile* getTile(int x, int y);

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;
    
    // Initialize the object by reading a maze file; true if successful, false otherwise
    bool initializeViaMazeFile();

    // Initialize the Maze object by randomly generating a maze
    void initializeViaMazeGenerator();

    // Initializes all of the tiles of the maze
    void initializeMaze(std::vector<std::vector<BasicTile>> maze);

    // Extracts the maze information
    std::vector<std::vector<BasicTile>> extractMaze();

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

} // namespace sim
