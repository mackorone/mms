#ifndef MAZE_H_
#define MAZE_H_

#include <string>
#include <vector>
#include "Tile.h"

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

#endif // MAZE_H_
