#pragma once

namespace mack {

class Cell {

public:
    Cell(int x, int y);
    int getX() const;
    int getY() const;
    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

private:
    int m_x;
    int m_y;
    bool m_walls[4];

};

} // namespace mack
