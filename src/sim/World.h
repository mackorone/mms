#pragma once

#include <mutex>
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

    void simulate();
    Seconds getElapsedRealTime() const;
    Seconds getElapsedSimTime() const;

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;
    WorldOptions m_options;

    mutable std::mutex m_updateMutex;
    Seconds m_elapsedRealTime;
    Seconds m_elapsedSimTime;

    void checkCollision();
};

} // namespace sim
