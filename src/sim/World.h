#pragma once

#include "Maze.h"
#include "Mouse.h"
#include "Polygon.h"
#include <vector>

namespace sim {

// The world is responsible for knowing the true position of the mouse and
// the maze, as well as carrying forth the simulation (i.e., stepping forward
// in time)
class World {

public:
    World(Maze* maze, Mouse* mouse);
    void simulate(); // TODO

    /* // TODO
    bool inGoal();
    void resetPosition(); // Sets the position of the mouse to (0,0)
    void resetColors(int curX, int curY); // Resets the tile colors of the maze
    */

private:
    Maze* m_maze;
    Mouse* m_mouse;

    // Whether or not the mouse has collided
    bool m_collision;
    void checkCollision();

};

} // namespace sim
