#pragma once

#include "CellList.h"

// TODO: Optimization
// - 1 byte for the location
// - 1 byte for the walls

namespace mack {

class Cell {

public:
    Cell();

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

private:
    int m_x;
    int m_y;
    bool m_walls[4];

};

} // namespace mack
