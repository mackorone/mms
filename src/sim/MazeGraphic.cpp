#include "MazeGraphic.h"

#include "Assert.h"

namespace mms {

MazeGraphic::MazeGraphic(
        const Maze* maze,
        BufferInterface* bufferInterface) {
    for (int x = 0; x < maze->getWidth(); x += 1) {
        QVector<TileGraphic> column;
        for (int y = 0; y < maze->getHeight(); y += 1) {
            column.append(TileGraphic(
                maze->getTile(x, y),
                bufferInterface));
        }
        m_tileGraphics.append(column);
    }
    ASSERT_EQ(getWidth(), maze->getWidth());
    ASSERT_EQ(getHeight(), maze->getHeight());
}

void MazeGraphic::setTileColor(int x, int y, Color color) {
    ASSERT_TR(withinMaze(x, y));
    m_tileGraphics[x][y].setColor(color);
}

void MazeGraphic::declareWall(int x, int y, Direction direction, bool isWall) {
    ASSERT_TR(withinMaze(x, y));
    m_tileGraphics[x][y].declareWall(direction, isWall);
}

void MazeGraphic::undeclareWall(int x, int y, Direction direction) {
    ASSERT_TR(withinMaze(x, y));
    m_tileGraphics[x][y].undeclareWall(direction);
}

void MazeGraphic::setTileText(int x, int y, const QString& text) {
    ASSERT_TR(withinMaze(x, y));
    m_tileGraphics[x][y].setText(text);
}

void MazeGraphic::drawPolygons() const {
    // Fill the GRAPHIC_CPU_BUFFER
    for (int x = 0; x < m_tileGraphics.size(); x += 1) {
        for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
            m_tileGraphics.at(x).at(y).drawPolygons();
        }
    }
}

void MazeGraphic::drawTextures() {
    // Fill the TEXTURE_CPU_BUFFER
    for (int x = 0; x < m_tileGraphics.size(); x += 1) {
        for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
            m_tileGraphics[x][y].drawTextures();
        }
    }
}

int MazeGraphic::getWidth() const {
    return m_tileGraphics.size();
}

int MazeGraphic::getHeight() const {
    return (m_tileGraphics.size() > 0 ? m_tileGraphics.at(0).size() : 0);
}

bool MazeGraphic::withinMaze(int x, int y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

} 
