#pragma once

namespace mack {

class Cell {

public:
    Cell();

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

    bool isWall(int direction) const;
    void setWall(int direction, bool isWall);

    int getDistance() const;
    void setDistance(int distance);

private:
    int m_x;
    int m_y;
    int m_distance;
    bool m_walls[4];
    //Cell* m_neighbors[4];

};

} // namespace mack
