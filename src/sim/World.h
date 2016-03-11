#pragma once

#include <set>
#include <vector>

#include "Maze.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "units/Seconds.h"
#include "WorldOptions.h"

namespace sim {

class World {

public:
    World(
        const Maze* maze,
        Mouse* mouse,
        MazeGraphic* mazeGraphic,
        WorldOptions options);

    int getNumberOfTilesTraversed() const;
    int getClosestDistanceToCenter() const;
    Seconds getBestTimeToCenter() const;
    Seconds getTimeSinceOriginDeparture() const;

    void simulate();

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;
    WorldOptions m_options;

    std::set<const Tile*> m_traversedTiles;
    int m_closestDistanceToCenter;
    Seconds m_bestTimeToCenter;
    Seconds m_timeOfOriginDeparture;

    void checkCollision();
};

} // namespace sim
