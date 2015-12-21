#pragma once

#include "Maze.h"
#include "Mouse.h"

namespace sim {

class Model {

public:
    Model();
    Maze* getMaze(); // TODO: MACK - get rid of these eventually
    Mouse* getMouse(); // TODO: MACK - get rid of these eventually

private:
    Maze* m_maze;
    Mouse* m_mouse;

};

} // namespace sim
