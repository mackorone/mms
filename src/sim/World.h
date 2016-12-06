#pragma once

#include <QPair>
#include <QSet>

#include "InterfaceType.h"
#include "Maze.h"
#include "Mouse.h"
#include "units/Seconds.h"

namespace mms {

class World {

public:
    World(const Maze* maze, Mouse* mouse);

    Seconds getBestTimeToCenter() const;
    Seconds getTimeSinceOriginDeparture() const;

    int getNumberOfTilesTraversed() const;
    int getClosestDistanceToCenter() const;

    void simulate();

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    
    Seconds m_bestTimeToCenter;
    Seconds m_timeOfOriginDeparture;

    QSet<QPair<int, int>> m_traversedTileLocations;
    int m_closestDistanceToCenter;

    void checkCollision();
};

} // namespace mms
