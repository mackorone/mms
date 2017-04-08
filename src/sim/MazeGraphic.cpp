#include "MazeGraphic.h"

#include "Assert.h"

namespace mms {

MazeGraphic::MazeGraphic(
        const Maze* maze,
        BufferInterface* bufferInterface,
        bool wallTruthVisible,
        bool tileColorsVisible,
        bool tileFogVisible,
        bool tileTextVisible,
        bool autopopulateTextWithDistance) {
    for (int x = 0; x < maze->getWidth(); x += 1) {
        QVector<TileGraphic> column;
        for (int y = 0; y < maze->getHeight(); y += 1) {
            column.push_back(TileGraphic(
                maze->getTile(x, y),
                bufferInterface,
                wallTruthVisible,
                tileColorsVisible,
                tileFogVisible,
                tileTextVisible,
                autopopulateTextWithDistance));
        }
        m_tileGraphics.push_back(column);
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

void MazeGraphic::setTileFogginess(int x, int y, bool foggy) {
    ASSERT_TR(withinMaze(x, y));
    m_tileGraphics[x][y].setFogginess(foggy);
}

void MazeGraphic::setTileText(int x, int y, const QString& text) {
    ASSERT_TR(withinMaze(x, y));
    m_tileGraphics[x][y].setText(text);
}

void MazeGraphic::setWallTruthVisible(bool visible) {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics[x][y].setWallTruthVisible(visible);
        }
    }
}

void MazeGraphic::setTileColorsVisible(bool visible) {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics[x][y].setTileColorsVisible(visible);
        }
    }
}

void MazeGraphic::setTileFogVisible(bool visible) {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics[x][y].setTileFogVisible(visible);
        }
    }
}

void MazeGraphic::setTileTextVisible(bool visible) {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics[x][y].setTileTextVisible(visible);
        }
    }
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

} // namespace mms
