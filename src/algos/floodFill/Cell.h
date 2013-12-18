#ifndef CELL_H_
#define CELL_H_

class Cell{

public:
    Cell();
    ~Cell();

    int distance();
    void setDistance(int distance);

    bool isWall(int direction);
    void setWall(int direction, bool isWall);

private:
    int m_distance;
    bool m_walls[4]; // North, East, South, West

};

#endif // CELL_H_
