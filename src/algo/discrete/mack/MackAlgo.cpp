#include "MackAlgo.h"

#include <iostream>

// XXX: Don't use STL

namespace mack {

void MackAlgo::solve(sim::MouseInterface* mouse) {

    // Initialization
    m_mouse = mouse;
    m_mouse->declareInterfaceType(sim::DISCRETE);
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            m_maze[x][y].setPosition(x, y);
        }
    }

    // Assign the neighbors of every cell
    // TODO: At the beginning of the maze, a good heuristic is to move towards the center if you can
    // BFS from every position, move towards center if possible
     
}

void MackAlgo::setColor(int x, int y, char color) {
    m_mouse->colorTile(x, y, color);
}

void MackAlgo::resetColors() {
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            setColor(x, y, 'k');
        }
    }
}


} // namespace mack
