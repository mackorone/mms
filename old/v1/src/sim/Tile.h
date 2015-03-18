#pragma once

#include <vector>

namespace sim {

/*
 *  A class that represents each individual tile in the maze.
 *
 *  Attributes:
 *      m_x: X position of the tile
 *      m_y: Y position of the tile
 *      m_distance: Distance of the tile from the starting tile (i.e., (0, 0))
 *      m_passes: The number of passes that the mouse has made over the tile
 *      m_explored: Whether or not the tile has been explored by the pathfinder yet
 *      m_posp: Whether or not the tile is part of the shortest path
 *      m_walls: The walls surrounding the tile
 *      m_neighbors: Neighboring tiles, i.e., able to be traveled to
 */
class Tile{

public:
    Tile();
    ~Tile();

    int getX();
    int getY();
    int getDistance();
    int getPasses();
    bool getExplored(); 
    bool getPosp(); 
    bool isWall(int direction);
    std::vector<Tile*> getNeighbors();

    void setPos(int x, int y);
    void setDistance(int distance);
    void incrementPasses();
    void resetPasses();
    void setExplored(bool explored);
    void setPosp(bool posp);
    void setWall(int wall, bool exists);
    void addNeighbor(Tile* neighbor);
    void resetNeighbors();

private:
    int m_x;
    int m_y;
    int m_distance;
    int m_passes;
    bool m_explored;
    bool m_posp;
    bool m_walls[4];
    std::vector<Tile*> m_neighbors;
};

} // namespace sim
