#pragma once

#include <QString>
#include <QVector>

#include "Tile.h"

namespace mms {

typedef QVector<QVector<QMap<Direction, bool>>> BasicMaze;

class Maze {
 public:
  static Maze *fromFile(const QString &path);

  int getWidth() const;
  int getHeight() const;
  const Tile *getTile(int x, int y) const;
  bool isInCenter(QPair<int, int> location) const;

 private:
  QVector<QVector<Tile>> m_tiles;
  explicit Maze(BasicMaze basicMaze);

  // Maze file formats
  static Maze *fromMapFile(QVector<QString> lines);
  static Maze *fromNumFile(QVector<QString> lines);

  // Validate the maze
  static bool isValid(const BasicMaze &basicMaze);
  static bool isNonempty(const BasicMaze &basicMaze);
  static bool isRectangular(const BasicMaze &basicMaze);
  static bool isEnclosed(const BasicMaze &basicMaze);
  static bool isConsistent(const BasicMaze &basicMaze);

  // Populate distances
  static QVector<QVector<int>> getDistances(BasicMaze basicMaze);
  static QVector<QPair<int, int>> getCenterPositions(int width, int height);
};

}  // namespace mms
