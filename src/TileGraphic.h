#pragma once

#include <QMap>
#include <QPair>

#include "BufferInterface.h"
#include "Color.h"
#include "Tile.h"

namespace mms {

class TileGraphic {
 public:
  TileGraphic();
  TileGraphic(const Tile *tile, BufferInterface *bufferInterface,
              bool isTruthView);

  void setWall(Direction direction);
  void clearWall(Direction direction);

  void setColor(const Color color);
  void clearColor();

  void setText(const QString &text);
  void clearText();

  // TODO: upforgrabs
  // Rename these to "reload" or something
  void drawPolygons() const;
  void drawTextures() const;

  void refreshColors();

 private:
  // Input and output objects
  const Tile *m_tile;
  BufferInterface *m_bufferInterface;

  // Visual state
  QMap<Direction, bool> m_walls;
  Color m_color;
  bool m_colorWasSet;
  QString m_text;

  // Helper functions
  // TODO: upforgrabs
  // Rename these to "refresh" or something
  void updateWall(Direction direction) const;
  void updateColor() const;
  void updateText() const;

  bool m_isTruthView;
  Color getWallColor(Direction direction) const;
  unsigned char getWallAlpha(Direction direction) const;
};

}  // namespace mms
