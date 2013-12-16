#ifndef MAZEGRAPHIC_H_
#define MAZEGRAPHIC_H_

#include <vector>
#include "Maze.h"
#include "TileGraphic.h"

class MazeGraphic{

public:
    MazeGraphic(Maze* maze);
    ~MazeGraphic();
    void draw();

private:
    Maze* m_maze;
    std::vector<std::vector<TileGraphic>> m_tileGraphics;
};

#endif // MAZEGRAPHIC_H_
