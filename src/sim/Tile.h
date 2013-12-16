#ifndef TILE_H_
#define TILE_H_

class Tile{

public:
    Tile();
    ~Tile();
    int getX();
    int getY();
    void setPos(int x, int y);
    bool isWall(int direction);
    void setWall(int wall, bool exists);
    int getPasses();
    void incrementPasses();

private:
    int m_x;
    int m_y;
    bool m_walls[4]; // NORTH, EAST, SOUTH, WEST
    int m_passes;
};

#endif // TILE_H_
