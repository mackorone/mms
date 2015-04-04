#pragma once

#include "CellList.h"
#include "Directions.h"

// TODO: Optimization
// - 1 byte for the location
// - 1 byte for the walls

namespace mack {

class Cell {

public:
    Cell();

    /*
    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
    */

    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

    //int getDistance() const;
    //void setDistance(int distance);
    
    //Cell* getNeighbor(int direction);
    //void setNeighbor(int direction, Cell* neighbor);

private:
    /*
    int m_x;
    int m_y;
    */
    //int m_distance;
    bool m_walls[4];
    //CellList m_neighbors;

};

} // namespace mack
