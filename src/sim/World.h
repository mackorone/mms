#pragma once

#include <set>

#include "Maze.h"
#include "Mouse.h"
#include "units/Seconds.h"

namespace sim {

class World {

public:
    World(const Maze* maze, Mouse* mouse);

    Seconds getBestSimTimeToCenter() const;
    Seconds getSimTimeSinceOriginDeparture() const;

    int getNumberOfTilesTraversed() const;
    int getClosestDistanceToCenter() const;

    void simulate();

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    
    Seconds m_elapsedSimTime;
    Seconds m_bestSimTimeToCenter;
    Seconds m_simTimeOfOriginDeparture;

    std::set<const Tile*> m_traversedTiles;
    int m_closestDistanceToCenter;

    void checkCollision();
};

} // namespace sim
