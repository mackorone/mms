#include "MazeGraphic.h"

#include "Assert.h"
#include "State.h"

namespace sim {

MazeGraphic::MazeGraphic(const Maze* maze) {
    for (int x = 0; x < maze->getWidth(); x += 1) {
        std::vector<TileGraphic> column;
        for (int y = 0; y < maze->getHeight(); y += 1) {
            column.push_back(TileGraphic(maze->getTile(x, y)));
        }
        m_tileGraphics.push_back(column);
    }
}

void MazeGraphic::draw() const {
    for (int x = 0; x < m_tileGraphics.size(); x += 1) {
        for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
            m_tileGraphics.at(x).at(y).draw();
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
    // TODO
    m_tileGraphics.at(x).at(y).draw();
}

void MazeGraphic::setTileText(int x, int y, const std::string& text) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).setText(text);
}

void MazeGraphic::setTileFogginess(int x, int y, bool foggy) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).setFogginess(foggy);
    // TODO
    m_tileGraphics.at(x).at(y).draw();
}

void MazeGraphic::declareWall(int x, int y, Direction direction, bool isWall) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).declareWall(direction, isWall);
    // TODO
    m_tileGraphics.at(x).at(y).draw();
}

void MazeGraphic::undeclareWall(int x, int y, Direction direction) {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    m_tileGraphics.at(x).at(y).undeclareWall(direction);
    // TODO
    m_tileGraphics.at(x).at(y).draw();
}

bool MazeGraphic::wallDeclared(int x, int y, Direction direction) const {
    ASSERT(0 <= x && x <= getWidth() && 0 <= y && y < getHeight());
    return m_tileGraphics.at(x).at(y).wallDeclared(direction);
}

} // namespace sim
