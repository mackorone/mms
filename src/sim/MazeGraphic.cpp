#include "MazeGraphic.h"

#include "Assert.h"

namespace sim {

MazeGraphic::MazeGraphic(const Maze* maze, BufferInterface* bufferInterface) {
    for (int x = 0; x < maze->getWidth(); x += 1) {
        std::vector<TileGraphic> column;
        for (int y = 0; y < maze->getHeight(); y += 1) {
            column.push_back(TileGraphic(maze->getTile(x, y), bufferInterface));
        }
        m_tileGraphics.push_back(column);
    }
    SIM_ASSERT_EQ(getWidth(), maze->getWidth());
    SIM_ASSERT_EQ(getHeight(), maze->getHeight());
}

void MazeGraphic::setTileColor(int x, int y, Color color) {
    SIM_ASSERT_TR(withinMaze(x, y));
    m_tileGraphics.at(x).at(y).setColor(color);
}

void MazeGraphic::declareWall(int x, int y, Direction direction, bool isWall) {
    SIM_ASSERT_TR(withinMaze(x, y));
    m_tileGraphics.at(x).at(y).declareWall(direction, isWall);
}

void MazeGraphic::undeclareWall(int x, int y, Direction direction) {
    SIM_ASSERT_TR(withinMaze(x, y));
    m_tileGraphics.at(x).at(y).undeclareWall(direction);
}

void MazeGraphic::setTileFogginess(int x, int y, bool foggy) {
    SIM_ASSERT_TR(withinMaze(x, y));
    m_tileGraphics.at(x).at(y).setFogginess(foggy);
}

void MazeGraphic::setTileText(int x, int y, const std::vector<std::string>& rowsOfText) {
    SIM_ASSERT_TR(withinMaze(x, y));
    m_tileGraphics.at(x).at(y).setText(rowsOfText);
}

void MazeGraphic::draw() const {

    // Make sure that this function is only called once
    SIM_ASSERT_RUNS_JUST_ONCE();

    // Fill the GRAPHIC_CPU_BUFFER
    for (int x = 0; x < m_tileGraphics.size(); x += 1) {
        for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
            m_tileGraphics.at(x).at(y).draw();
        }
    }
}

void MazeGraphic::updateColor() const {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics.at(x).at(y).updateColor();
        }
    }
}

void MazeGraphic::updateWalls() const {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics.at(x).at(y).updateWalls();
        }
    }
}

void MazeGraphic::updateFog() const {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics.at(x).at(y).updateFog();
        }
    }
}

void MazeGraphic::updateText() const {
    for (int x = 0; x < getWidth(); x += 1) {
        for (int y = 0; y < getHeight(); y += 1) {
            m_tileGraphics.at(x).at(y).updateText();
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

} // namespace sim
