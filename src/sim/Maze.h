#pragma once

#include <string>
#include <vector>

#include "Tile.h"

namespace sim {

class Maze {

public:
    Maze();

    int getWidth(); // TODO: Needed?
    int getHeight(); // TODO: Needed?
    Tile* getTile(int x, int y);

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;
    
    // -------------------- Maze generation utilities -----------------------//

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

    // Saves the mazefile into the file given by mazefile
    void saveMaze(std::string mazeFile);

    // Loads the mazefile given by mazefile
    void loadMaze(std::string mazeFile);
};

} // namespace sim
