#pragma once

#include <set>

#include "InterfaceType.h"
#include "Maze.h"
#include "Mouse.h"
#include "units/Seconds.h"

namespace sim {

class World {

public:
    World(const Maze* maze, Mouse* mouse);

    Seconds getBestTimeToCenter() const;
    Seconds getTimeSinceOriginDeparture() const;

    int getNumberOfTilesTraversed() const;
    int getClosestDistanceToCenter() const;

    void setInterfaceType(InterfaceType interfaceType);
    InterfaceType getInterfaceType() const;

    void simulate();

private:
    const Maze* m_maze;
    Mouse* m_mouse;

    InterfaceType m_interfaceType;
    
    Seconds m_bestTimeToCenter;
    Seconds m_timeOfOriginDeparture;

    std::set<std::pair<int, int>> m_traversedTileLocations;
    int m_closestDistanceToCenter;

    void checkCollision();
};

} // namespace sim
