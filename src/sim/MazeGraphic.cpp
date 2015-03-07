#include "MazeGraphic.h"

namespace sim {

MazeGraphic::MazeGraphic(const Maze* maze) {
    for (int i = 0; i < maze->getWidth(); i += 1) {
        std::vector<TileGraphic> column;
        for (int j = 0; j < maze->getHeight(); j += 1) {
            column.push_back(TileGraphic(maze->getTile(i, j)));
        }
        m_tileGraphics.push_back(column);
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
