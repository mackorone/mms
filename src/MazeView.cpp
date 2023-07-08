#include "MazeView.h"

#include "BufferInterface.h"
#include "Dimensions.h"
#include "MazeGraphic.h"

namespace mms {

MazeView::MazeView(const Maze *maze, bool isTruthView)
    : m_bufferInterface({maze->getWidth(), maze->getHeight()},
                        &m_graphicCpuBuffer, &m_textureCpuBuffer),
      m_mazeGraphic(maze, &m_bufferInterface, isTruthView) {
  // Establish the coordinates for the tile text characters
  initText(2, 5);

  // Populate the data vectors with wall polygons and tile distance text.
  m_mazeGraphic.drawPolygons();
  m_mazeGraphic.drawTextures();
}

MazeGraphic *MazeView::getMazeGraphic() { return &m_mazeGraphic; }

void MazeView::initTileGraphicText(int numRows, int numCols) {
  initText(numRows, numCols);
}

const QVector<TriangleGraphic> *MazeView::getGraphicCpuBuffer() const {
  return &m_graphicCpuBuffer;
}

const QVector<TriangleTexture> *MazeView::getTextureCpuBuffer() const {
  return &m_textureCpuBuffer;
}

void MazeView::initText(int numRows, int numCols) {
  // Initialze the tile text in the buffer class,
  // do caching for speed improvement
  m_bufferInterface.initTileGraphicText(
      Dimensions::wallLength(), Dimensions::wallWidth(), {numRows, numCols});

  // TODO: upforgrabs
  // The naming ("draw") is kind of confusing
  m_textureCpuBuffer.clear();
  m_mazeGraphic.drawTextures();
}

}  // namespace mms
