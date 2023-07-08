#pragma once

#include <QChar>
#include <QPair>
#include <QVector>

#include "Color.h"
#include "Direction.h"
#include "Polygon.h"
#include "TileGraphicTextCache.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace mms {

class BufferInterface {
 public:
  BufferInterface(QPair<int, int> mazeSize,
                  QVector<TriangleGraphic> *graphicCpuBuffer,
                  QVector<TriangleTexture> *textureCpuBuffer);

  // Initializes and caches all possible tile text positions. We need this
  // extra initialization function since the max size is from the algorithm.
  void initTileGraphicText(const Distance &wallLength,
                           const Distance &wallWidth,
                           QPair<int, int> tileGraphicTextMaxSize);

  // Returns the maximum number of rows and columns of text in a tile graphic
  QPair<int, int> getTileGraphicTextMaxSize();

  // Fills the graphic cpu buffer and texture cpu buffer
  void insertIntoGraphicCpuBuffer(const Polygon &polygon, Color color,
                                  unsigned char alpha);
  void insertIntoTextureCpuBuffer();

  // These methods are inexpensive, and may be called many times
  void updateTileGraphicBaseColor(int x, int y, Color color);
  void updateTileGraphicWallColor(int x, int y, Direction direction,
                                  Color color, unsigned char alpha);
  void updateTileGraphicText(int x, int y, int numRows, int numCols, int row,
                             int col, QChar c);

 private:
  // The width and height of the maze
  QPair<int, int> m_mazeSize;

  // CPU-side buffers
  QVector<TriangleGraphic> *m_graphicCpuBuffer;
  QVector<TriangleTexture> *m_textureCpuBuffer;

  // A cache for tile graphic text information
  TileGraphicTextCache m_tileGraphicTextCache;

  // Retrieve the indices into the graphic cpu buffer,
  // for each specific type of Tile triangle
  int trianglesPerTile();
  int getTileGraphicBaseStartingIndex(int x, int y);
  int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
  int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);

  // Retrieve the indices into the texture cpu buffer
  int getTileGraphicTextStartingIndex(int x, int y, int row, int col);
};

}  // namespace mms
