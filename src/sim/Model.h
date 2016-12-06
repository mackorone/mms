#pragma once

#include "Maze.h"
#include "Mouse.h"
#include "World.h"

namespace mms {

class Model {

public:

    Model();

    const Maze* getMaze() const;
    const Mouse* getMouse() const;

    Mouse* getMouse();
    World* getWorld();

private:

    Maze* m_maze;
    Mouse* m_mouse;
    World* m_world;

};

} // namespace mms
