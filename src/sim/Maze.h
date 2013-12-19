#ifndef SIM_MAZE_H_
#define SIM_MAZE_H_

#include <string>
#include <vector>
#include "Tile.h"

namespace sim{

class Maze{

public:
    Maze(int width, int height, std::string mazeFile = "");
    ~Maze();
    int getWidth();
    int getHeight();
    Tile* getTile(int xPos, int yPos);
    void randomize();
    void saveMaze(std::string mazeFile);
    void loadMaze(std::string mazeFile);

private:
    std::vector<std::vector<Tile>> m_maze;
};

} // namespace sim

#endif // SIM_MAZE_H_
