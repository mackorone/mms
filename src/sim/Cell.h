#ifndef SIM_CELL_H_
#define SIM_CELL_H_

#include <vector>

namespace gen {

/*
 *  A class that represents each individual cell in the maze.
 *
 *  Attributes:
 *      m_x: X position of the Cell
 *      m_y: Y position of the Cell
 *      m_distance: The distance of the cell from the start of the maze along the tree
 *      m_explored: -----------------------------------------------------
 *      m_walls: The walls surrounding the Cell
 */

class Cell{

public:
    Cell();
    ~Cell();

    int getX();
    int getY();
    int getDistance();
    bool getExplored(); 

    void setExplored(bool explored);
    void setDistance(int distance);
    void setWall(int wall, bool exists);

private:
    int m_x;
    int m_y;
    int m_distance;
    bool m_explored;
    bool m_walls[4];
};

} // namespace sim

#endif // SIM_TILE_H_
