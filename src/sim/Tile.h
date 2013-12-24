#ifndef SIM_TILE_H_
#define SIM_TILE_H_

namespace sim{

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
    bool getPosp(); // Part-Of-Shortest-Path
    void setPosp(bool posp);

private:
    int m_x;
    int m_y;
    bool m_walls[4]; // NORTH, EAST, SOUTH, WEST
    int m_passes;
    bool m_posp;
};

} // namespace sim

#endif // SIM_TILE_H_
