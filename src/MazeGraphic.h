#ifndef MAZEGRAPHIC_H_
#define MAZEGRAPHIC_H_

#include "Maze.h"

class MazeGraphic{

public:
    MazeGraphic(Maze* maze);
    ~MazeGraphic();
    void draw();

private:
    Maze* m_maze;
};

#endif // MAZEGRAPHIC_H_
