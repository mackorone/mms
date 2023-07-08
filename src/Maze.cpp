#include "Maze.h"

#include <QFile>
#include <QQueue>
#include <QTextStream>

#include "AssertMacros.h"

namespace mms {

Maze *Maze::fromFile(const QString &path) {
  // Open the file
  if (path.isEmpty()) {
    return nullptr;
  }
  QFile file(path);
  if (!file.open(QFile::ReadOnly)) {
    return nullptr;
  }

  // Read the lines into a vector
  QString line;
  QVector<QString> lines;
  QTextStream stream(&file);
  while (stream.readLineInto(&line)) {
    lines.append(line);
  }

  // Try map format first, then num
  Maze *maze = fromMapFile(lines);
  if (maze != nullptr) {
    return maze;
  }
  return fromNumFile(lines);
}

int Maze::getWidth() const { return m_tiles.size(); }

int Maze::getHeight() const { return m_tiles.at(0).size(); }

const Tile *Maze::getTile(int x, int y) const {
  ASSERT_LE(0, x);
  ASSERT_LE(0, y);
  ASSERT_LT(x, getWidth());
  ASSERT_LT(y, getHeight());
  return &m_tiles.at(x).at(y);
}

bool Maze::isInCenter(QPair<int, int> location) const {
  return getCenterPositions(getWidth(), getHeight()).contains(location);
}

Maze::Maze(BasicMaze basicMaze) {
  QVector<QVector<int>> distances = getDistances(basicMaze);
  for (int x = 0; x < basicMaze.size(); x += 1) {
    QVector<Tile> column;
    for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
      Tile tile(x, y, distances.at(x).at(y), basicMaze.at(x).at(y));
      tile.initPolygons(basicMaze.size(), basicMaze.at(x).size());
      column.append(tile);
    }
    m_tiles.append(column);
  }
}

Maze *Maze::fromMapFile(QVector<QString> lines) {
  // Format:
  //
  //     +---+---+---+
  //     |       |   |
  //     +   +   +   +
  //     |   |       |
  //     +---+---+---+

  // Flip the lines to process them from bottom to top
  for (int i = 0; i < lines.size() / 2; i += 1) {
    int j = lines.size() - 1 - i;
    QString one = lines.at(i);
    QString two = lines.at(j);
    lines[i] = two;
    lines[j] = one;
  }

  BasicMaze basicMaze;
  for (int y = 0; y < lines.size() / 2; y += 1) {
    for (int x = 0; x < lines.at(y).size() / 4; x += 1) {
      // Fill out the maze as necessary
      while (basicMaze.size() <= x) {
        QVector<QMap<Direction, bool>> column;
        basicMaze.append(column);
      }
      while (basicMaze.at(x).size() <= y) {
        QMap<Direction, bool> walls;
        basicMaze[x].append(walls);
      }

      // Calculate the edges of the cell:
      //
      //    west v
      //         +---+ < north
      //         |   |
      // south > +---+
      //             ^ east
      //
      int north = 2 * (y + 1);
      int south = 2 * (y + 0);
      int east = 4 * (x + 1);
      int west = 4 * (x + 0);

      // Check bounds
      if (lines.size() <= north) {
        return nullptr;
      }
      if (lines.at(south + 1).size() <= east) {
        return nullptr;
      }

      // Add values for the current cell
      QMap<Direction, bool> walls;
      walls[Direction::NORTH] = lines.at(north).at(west + 2) != ' ';
      walls[Direction::EAST] = lines.at(south + 1).at(east) != ' ';
      walls[Direction::SOUTH] = lines.at(south).at(west + 2) != ' ';
      walls[Direction::WEST] = lines.at(south + 1).at(west) != ' ';
      basicMaze[x][y] = walls;
    }
  }

  // Check if the maze is valid
  if (!isValid(basicMaze)) {
    return nullptr;
  }

  return new Maze(basicMaze);
}

Maze *Maze::fromNumFile(QVector<QString> lines) {
  // Format:
  //
  //     X Y N E S W
  //
  // Example:
  //
  //     0 0 0 1 1 1
  //     0 1 1 0 0 1
  //     1 0 0 0 1 1
  //     1 1 1 1 0 0
  //     2 0 0 1 1 0
  //     2 1 1 1 0 1
  //
  // Result:
  //
  //     +---+---+---+
  //     |       |   |
  //     +   +   +   +
  //     |   |       |
  //     +---+---+---+

  BasicMaze basicMaze;
  for (QString line : lines) {
    // Tokenize the line
    QStringList tokens = line.split(" ", Qt::SkipEmptyParts);
    if (tokens.size() != 6) {
      return nullptr;
    }

    // Extract numeric values
    bool ok = true;
    int x = tokens.at(0).toInt(&ok);
    int y = tokens.at(1).toInt(&ok);
    bool n = tokens.at(2).toInt(&ok) == 1;
    bool e = tokens.at(3).toInt(&ok) == 1;
    bool s = tokens.at(4).toInt(&ok) == 1;
    bool w = tokens.at(5).toInt(&ok) == 1;
    if (!ok) {
      return nullptr;
    }

    // Fill out the maze as necessary
    while (basicMaze.size() <= x) {
      QVector<QMap<Direction, bool>> column;
      basicMaze.append(column);
    }
    while (basicMaze.at(x).size() <= y) {
      QMap<Direction, bool> walls;
      basicMaze[x].append(walls);
    }

    // Add values for the current cell
    QMap<Direction, bool> walls;
    walls[Direction::NORTH] = n;
    walls[Direction::EAST] = e;
    walls[Direction::SOUTH] = s;
    walls[Direction::WEST] = w;
    basicMaze[x][y] = walls;
  }

  // Check if the maze is valid
  if (!isValid(basicMaze)) {
    return nullptr;
  }

  return new Maze(basicMaze);
}

bool Maze::isValid(const BasicMaze &basicMaze) {
  return (isNonempty(basicMaze) && isRectangular(basicMaze) &&
          isEnclosed(basicMaze) && isConsistent(basicMaze));
}

bool Maze::isNonempty(const BasicMaze &basicMaze) {
  return 0 < basicMaze.size() && 0 < basicMaze.at(0).size();
}

bool Maze::isRectangular(const BasicMaze &basicMaze) {
  for (int x = 0; x < basicMaze.size() - 1; x += 1) {
    if (basicMaze.at(x).size() != basicMaze.at(x + 1).size()) {
      return false;
    }
  }
  return true;
}

bool Maze::isEnclosed(const BasicMaze &basicMaze) {
  int width = basicMaze.size();
  int height = basicMaze.at(0).size();
  for (int x = 0; x < width; x += 1) {
    for (int y = 0; y < height; y += 1) {
      QMap<Direction, bool> walls = basicMaze.at(x).at(y);
      if (x == 0 && !walls.value(Direction::WEST)) {
        return false;
      }
      if (y == 0 && !walls.value(Direction::SOUTH)) {
        return false;
      }
      if (x == width - 1 && !walls.value(Direction::EAST)) {
        return false;
      }
      if (y == height - 1 && !walls.value(Direction::NORTH)) {
        return false;
      }
    }
  }
  return true;
}

bool Maze::isConsistent(const BasicMaze &basicMaze) {
  int width = basicMaze.size();
  int height = basicMaze.at(0).size();
  for (int x = 0; x < basicMaze.size(); x += 1) {
    for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
      QMap<Direction, bool> walls = basicMaze.at(x).at(y);
      if (0 < x && walls.value(Direction::WEST) &&
          !basicMaze.at(x - 1).at(y).value(Direction::EAST)) {
        return false;
      }
      if (0 < y && walls.value(Direction::SOUTH) &&
          !basicMaze.at(x).at(y - 1).value(Direction::NORTH)) {
        return false;
      }
      if (x < width - 1 && walls.value(Direction::EAST) &&
          !basicMaze.at(x + 1).at(y).value(Direction::WEST)) {
        return false;
      }
      if (y < height - 1 && walls.value(Direction::NORTH) &&
          !basicMaze.at(x).at(y + 1).value(Direction::SOUTH)) {
        return false;
      }
    }
  }
  return true;
}

QVector<QVector<int>> Maze::getDistances(BasicMaze basicMaze) {
  // Initialize all positions with default value
  QVector<QVector<int>> distances;
  for (int x = 0; x < basicMaze.size(); x += 1) {
    QVector<int> column;
    for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
      column.append(-1);
    }
    distances.append(column);
  }

  // Set the distances of the center positions to 0 and enqueue them
  QQueue<QPair<int, int>> discovered;
  int width = basicMaze.size();
  int height = basicMaze.at(0).size();
  for (QPair<int, int> position : getCenterPositions(width, height)) {
    distances[position.first][position.second] = 0;
    discovered.enqueue(position);
  }

  // Perform a breadth first search
  while (!discovered.empty()) {
    QPair<int, int> position = discovered.dequeue();
    int x = position.first;
    int y = position.second;
    for (Direction direction : CARDINAL_DIRECTIONS()) {
      if (!basicMaze.at(x).at(y).value(direction)) {
        int nx = x;
        int ny = y;
        if (direction == Direction::NORTH) {
          ny += 1;
        }
        if (direction == Direction::EAST) {
          nx += 1;
        }
        if (direction == Direction::SOUTH) {
          ny -= 1;
        }
        if (direction == Direction::WEST) {
          nx -= 1;
        }
        if (distances.at(nx).at(ny) == -1) {
          distances[nx][ny] = distances.at(x).at(y) + 1;
          discovered.enqueue({nx, ny});
        }
      }
    }
  }
  return distances;
}

QVector<QPair<int, int>> Maze::getCenterPositions(int width, int height) {
  // +---+---+
  // | C | D |
  // +---+---+
  // | A | B |
  // +---+---+
  QPair<int, int> A = {(width - 1) / 2, (height - 1) / 2};
  QPair<int, int> B = {width / 2, (height - 1) / 2};
  QPair<int, int> C = {(width - 1) / 2, height / 2};
  QPair<int, int> D = {width / 2, height / 2};

  QVector<QPair<int, int>> positions;
  positions.append(A);
  if (width % 2 == 0 && height % 2 == 0) {
    positions.append(B);
    positions.append(C);
    positions.append(D);
  } else if (width % 2 == 0) {
    positions.append(B);
  } else if (height % 2 == 0) {
    positions.append(C);
  }
  return positions;
}

}  // namespace mms
