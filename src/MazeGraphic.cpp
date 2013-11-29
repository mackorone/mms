#include "MazeGraphic.h"

#include "Parameters.h"
#include "TileGraphic.h"

MazeGraphic::MazeGraphic(Maze* maze) : m_maze(maze)
{ }

MazeGraphic::~MazeGraphic()
{ }

void MazeGraphic::draw(){
    for (int i = 0; i < MAZE_WIDTH; i++){
        for (int j = 0; j < MAZE_HEIGHT; j++){
            TileGraphic tileGraphic(m_maze->getTile(i, j));
            tileGraphic.draw();
        }
    }
}
