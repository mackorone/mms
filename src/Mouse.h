#pragma once

#include <QMap>
#include <QMutex>
#include <QPair>
#include <QString>
#include <QVector>

#include "Direction.h"
#include "Maze.h"
#include "Polygon.h"
#include "units/Coordinate.h"

namespace mms {

struct SemiPosition {
  // In most contexts, (x, y) coordinates refer to cell positions within the
  // maze, in which the bottom left cell has coordinates (0, 0). Within this
  // struct, however, (x, y) coordinates refer to semi-positions within the
  // maze. Here are the semi-positions for the bottom left cell of the maze.
  //
  //        |               |
  //      (0,2)---(1,2)---(2,2)-
  //        |               |
  //        |               |
  //        |               |
  //      (0,1)   (1,1)   (2,1)-
  //        |               |
  //        |               |
  //        |               |
  //      (0,0)---(1,0)---(2,0)-
  //
  // These semi-positions allow the mouse to traverse the maze in ordinal
  // directions (northeast, northwest, etc.)
  int x;
  int y;

  QPair<int, int> toMazeLocation();
};

class Mouse {
 public:
  Mouse();

  // Resets the mouse to the beginning of the maze
  void reset();

  // Sets the current translation and rotation of the mouse
  void teleport(const Coordinate &translation, const Angle &rotation);

  // Gets the current discretized translation and rotation of the mouse
  SemiPosition getCurrentDiscretizedTranslation() const;
  SemiDirection getCurrentDiscretizedRotation() const;

  // Retrieves the polygon of just the body of the mouse
  Polygon getCurrentBodyPolygon() const;
  Polygon getCurrentWheelPolygon() const;

 private:
  // The translation and rotation of the mouse
  Coordinate m_initialTranslation;
  Coordinate m_currentTranslation;
  Angle m_initialRotation;
  Angle m_currentRotation;

  // The parts of the mouse at the starting location
  Polygon m_initialBodyPolygon;
  Polygon m_initialWheelPolygon;
  Polygon getCurrentPolygon(const Polygon &initialPolygon) const;
};

}  // namespace mms
