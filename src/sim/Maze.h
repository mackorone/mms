#ifndef SIM_MAZE_H_
#define SIM_MAZE_H_

#include <string>
#include <vector>
#include "Tile.h"
#include "PathFinder.h"

namespace sim{

class Maze{

public:
    Maze(int width, int height, std::string mazeFile = "");
    ~Maze();

    int getWidth();
    int getHeight();
    Tile* getTile(int x, int y);

private:
    // Vector to hold all of the tiles
    std::vector<std::vector<Tile>> m_maze;

    // -------------------- Maze generation utilities -----------------------//

    // Generates a random maze based on the (hard-coded) probability that any
    // one wall will exist within the maze
    void randomize();

    // Saves the mazefile into the file given by mazefile
    void saveMaze(std::string mazeFile);

    // Loads the mazefile given by mazefile
    void loadMaze(std::string mazeFile);

    // Assigns neighbors to all of the tiles
    void assignNeighbors();


    // ---------------------- Path finding utilities ------------------------//

    // Callable function to assign posp values to all nodes
    bool solveShortestPath();

    // Prints the distance values of all of the Tiles
    void printDistances();

    // Assigns distance values to all tiles corresponding to the distance
    // from each tile to the tile at location (x, y)
    void setDistancesFrom(int x, int y);

    // Returns the center tile of the maze with the lowest distance value
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

#endif // SIM_MAZE_H_
