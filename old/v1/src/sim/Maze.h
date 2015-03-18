#pragma once

#include <string>
#include <vector>

#include "../sim/Tile.h"

namespace sim {

class Maze {

public:
    Maze(int width, int height, std::string mazeFileDirPath = "", std::string mazeFile = "");
    ~Maze();

    int getWidth();
    int getHeight();
    Tile* getTile(int x, int y);
    void resetColors(int curX, int curY); // Resets the colors of the maze

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;
    
    // -------------------- Maze generation utilities -----------------------//

    // Randomly generate a maze
    void randomize();
    void tom_random();

    // Saves the mazefile into the file given by mazefile
    void saveMaze(std::string mazeFile);

    // Loads the mazefile given by mazefile
    void loadMaze(std::string mazeFile);

    // Assigns neighbors to all of the tiles
    void assignNeighbors();


    // ---------------------- Path finding utilities ------------------------//

    // Callable function to assign posp values to all nodes. It returns a vector
    // of size two. The first value is whether or not the maze is solvable, and
    // the second value is whether or not it meets the minimum distance
    // requirements.
    std::vector<bool> solveShortestPath();

    // Prints the distance values (from (0, 0)) of all of the Tiles
    void printDistances();

    // Assigns distance values to all tiles corresponding to the distance
    // from each tile to the tile at location (x, y)
    void setDistancesFrom(int x, int y);

    // Returns the center tile of the maze with the lowest distance value
    // TODO: If two or more center tiles have equal distance values, it only
    //       returns the first one ....
    Tile* getClosestCenterTile();

    // Generates a vector of tiles that are part of the shortest path (posp),
    // but only after distance values have been assigned to the tiles
    std::vector<Tile*> backtrace(Tile* end);

    // Finds the shortest path, assigns distance values, and returns posp nodes
    // corresponding to the starting and ending locations of the maze
    std::vector<Tile*> findPathToCenter();

    // Finds the shortest path, assigns distance values, and returns posp nodes
    // corresponding to the path from (x1, y1) to (x2, y2)
    std::vector<Tile*> findPath(int x1, int y1, int x2, int y2);
};

} // namespace sim
