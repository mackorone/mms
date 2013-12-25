#if(0)
//#ifndef SIM_PATHFINDER_H_
#define SIM_PATHFINDER_H_

namespace sim{

/*
 * This class will be responsilble for finding the shortest path in the maze
 * and manipulating the data of the tiles within the maze to reflect the 
 * shortest path
 */
class PathFinder{

public:
    PathFinder(Maze* maze);
    ~PathFinder();

    // Outside callable function to assign posp values to all nodes
    void solveShortestPath();

private:
    Maze* m_maze; // A pointer to the maze for checking and manipulating tiles

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

#endif // SIM_PATHFINDER_H_

