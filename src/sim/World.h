#pragma once

#include <vector>

#include "Maze.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "Polygon.h"

namespace sim {

class World {

public:
    World(Maze* maze, Mouse* mouse);
    void simulate();

private:
    Maze* m_maze;
    Mouse* m_mouse;

    // Whether or not the mouse has collided
    bool m_collision;
    void checkCollision();
};

} // namespace sim
