#pragma once

#include <QMap>
#include <QVector>

#include "Direction.h"

namespace sim {

typedef QMap<Direction, bool> BasicTile;
typedef QVector<QVector<BasicTile>> BasicMaze;

} // namespace sim
