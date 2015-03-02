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
    float getX() { return 0; }
    float getY() { return 0; }
    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void resetPosition(); // Sets the position of the mouse to (0,0)
    void resetColors(int curX, int curY); // Resets the tile colors of the maze
    */

private:
    void checkCollision(); // TODO make this prettier
    bool checkTileCollision(Tile* tile, std::vector<Polygon> mouseShapes);
    Maze* m_maze; // TODO: Need this be persistent???
    Mouse* m_mouse;
    bool m_collision;

};

} // namespace sim
