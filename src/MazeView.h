#pragma once

#include <QVector>

#include "BufferInterface.h"
#include "Maze.h"
#include "MazeGraphic.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace mms {

class MazeView {
 public:
  MazeView(const Maze *maze, bool isTruthView);
  MazeGraphic *getMazeGraphic();
  void initTileGraphicText(int numRows, int numCols);
  const QVector<TriangleGraphic> *getGraphicCpuBuffer() const;
  const QVector<TriangleTexture> *getTextureCpuBuffer() const;

 private:
  // These vectors contain the triangles that will actually be drawn
  QVector<TriangleGraphic> m_graphicCpuBuffer;
  QVector<TriangleTexture> m_textureCpuBuffer;

  // The buffer interface provides abstractions which the MazeGraphic
  // uses to populate the vector of TriangleGraphic objects
  BufferInterface m_bufferInterface;

  // The MazeGraphic is essentially a "handle" into the above vectors;
  // it provides a high-level API for modifying their contents
  MazeGraphic m_mazeGraphic;

  // Helper method for initializing TileGraphic text
  void initText(int numRows, int numCols);
};

}  // namespace mms
