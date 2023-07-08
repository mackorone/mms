#include "MazeGraphic.h"

#include "AssertMacros.h"

namespace mms {

MazeGraphic::MazeGraphic(const Maze *maze, BufferInterface *bufferInterface,
                         bool isTruthView) {
  for (int x = 0; x < maze->getWidth(); x += 1) {
    QVector<TileGraphic> column;
    for (int y = 0; y < maze->getHeight(); y += 1) {
      column.append(
          TileGraphic(maze->getTile(x, y), bufferInterface, isTruthView));
    }
    m_tileGraphics.append(column);
  }
}

void MazeGraphic::setWall(int x, int y, Direction direction) {
  m_tileGraphics[x][y].setWall(direction);
}

void MazeGraphic::clearWall(int x, int y, Direction direction) {
  m_tileGraphics[x][y].clearWall(direction);
}

void MazeGraphic::setColor(int x, int y, Color color) {
  m_tileGraphics[x][y].setColor(color);
}

void MazeGraphic::clearColor(int x, int y) {
  m_tileGraphics[x][y].clearColor();
}

void MazeGraphic::setText(int x, int y, const QString &text) {
  m_tileGraphics[x][y].setText(text);
}

void MazeGraphic::clearText(int x, int y) { m_tileGraphics[x][y].clearText(); }

void MazeGraphic::drawPolygons() const {
  // Fill the GRAPHIC_CPU_BUFFER
  for (int x = 0; x < m_tileGraphics.size(); x += 1) {
    for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
      m_tileGraphics.at(x).at(y).drawPolygons();
    }
  }
}

void MazeGraphic::drawTextures() const {
  // Fill the TEXTURE_CPU_BUFFER
  for (int x = 0; x < m_tileGraphics.size(); x += 1) {
    for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
      m_tileGraphics[x][y].drawTextures();
    }
  }
}

void MazeGraphic::refreshColors() {
  for (int x = 0; x < m_tileGraphics.size(); x += 1) {
    for (int y = 0; y < m_tileGraphics.at(x).size(); y += 1) {
      m_tileGraphics[x][y].refreshColors();
    }
  }
}

}  // namespace mms
