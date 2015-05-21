#include "MazeGraphic.h"

#include "Assert.h"
#include "State.h"

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

void MazeGraphic::draw() const {

    // Only draw the maze if it's visible
    if (!S()->mazeVisible()) {
        return;
    }

    for (int i = 0; i < m_tileGraphics.size(); i += 1) {
        for (int j = 0; j < m_tileGraphics.at(i).size(); j += 1) {
            m_tileGraphics.at(i).at(j).draw();
        }
    }
}

int MazeGraphic::getWidth() const {
    return m_tileGraphics.size();
}

int MazeGraphic::getHeight() const {
    return (m_tileGraphics.size() > 0 ? m_tileGraphics.at(0).size() : 0);
}

void MazeGraphic::setTileColor(int x, int y, const GLfloat* color) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).setColor(color);
}

void MazeGraphic::setTileText(int x, int y, const std::string& text) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).setText(text);
}

void MazeGraphic::declareWall(int x, int y, Direction direction, bool isWall) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).declareWall(direction, isWall);
}

void MazeGraphic::undeclareWall(int x, int y, Direction direction) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).undeclareWall(direction);
}

} // namespace sim
