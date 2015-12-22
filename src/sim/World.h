#pragma once

#include <vector>

#include "Maze.h"
#include "MazeGraphic.h"
#include "Mouse.h"
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

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;
    WorldOptions m_options;

    void checkCollision();
};

} // namespace sim
