#include "MazeGraphic.h"

#include "Parameters.h"
#include "TileGraphic.h"

MazeGraphic::MazeGraphic(Maze* maze) : m_maze(maze){

    // Create vectors of value objects that hold references to Tiles
    for (int i = 0; i < maze->getWidth(); i++){
        std::vector<TileGraphic> col;
        for (int j = 0; j < maze->getHeight(); j++){
            col.push_back(TileGraphic(m_maze->getTile(i, j)));
        }
        m_tileGraphics.push_back(col);
    }
}

MazeGraphic::~MazeGraphic()
{ }

void MazeGraphic::draw(){
    for (int i = 0; i < m_maze->getWidth(); i++){
        for (int j = 0; j < m_maze->getHeight(); j++){
            m_tileGraphics.at(i).at(j).draw();
        }
    }
}
