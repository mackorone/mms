#include "MazeGraphic.h"

namespace sim{

MazeGraphic::MazeGraphic(Maze* maze) {

    // Create vectors of value objects that hold references to Tiles
    for (int i = 0; i < maze->getWidth(); i += 1) {
        std::vector<TileGraphic> col;
        for (int j = 0; j < maze->getHeight(); j += 1) {
            col.push_back(TileGraphic(maze->getTile(i, j)));
        }
        m_tileGraphics.push_back(col);
    }
}

void MazeGraphic::draw() {
    for (int i = 0; i < m_tileGraphics.size(); i += 1) {
        for (int j = 0; j < m_tileGraphics.at(i).size(); j += 1) {
            m_tileGraphics.at(i).at(j).draw();
        }
    }
}

} // namespace sim
