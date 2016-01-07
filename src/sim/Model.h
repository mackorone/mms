#pragma once

#include "Maze.h"
#include "Mouse.h"

namespace sim {

class Model {

public:
    Model();
    Maze* getMaze();
    Mouse* getMouse();

private:
    Maze* m_maze;
    Mouse* m_mouse;

};

} // namespace sim
