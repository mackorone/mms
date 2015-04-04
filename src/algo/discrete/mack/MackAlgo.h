#pragma once

#include "../../IAlgorithm.h"

#include "Cell.h"

namespace mack {

static const int MAZE_WIDTH = 16;
static const int MAZE_HEIGHT = 16;

class MackAlgo : IAlgorithm {

// TODO: All paths w/o a cycle to the center - determine the shortest with the edge weights

public:
    void solve(sim::MouseInterface* mouse);

private:
    sim::MouseInterface* m_mouse;
    Cell m_maze[MAZE_WIDTH][MAZE_HEIGHT];

    void setColor(int x, int y, char color);
    void resetColors();

    // Floodfill explore
    //void walls();
    //void flood(int x, int y);
    //void moveTowardsGoal();
    //bool inGoal(int x, int y);

    // BFS explore (try this against floodfill)


};

} // namespace mack
