#ifndef TILE_H_
#define TILE_H_

// TODO: Keep track of how many times the mouse passes over the tile

class Tile{

public:
    Tile();
    ~Tile();
    int getX();
    int getY();
    void setPos(int x, int y);
    bool isWall(int direction);
    //void setWalls(bool* walls); // TODO
    void setWall(int wall, bool exists);

private:
    int m_x;
    int m_y;
    bool m_walls[4]; // NORTH, EAST, SOUTH, WEST
};

#endif // TILE_H_
