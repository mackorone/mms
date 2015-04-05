#pragma once

#include "../../IAlgorithm.h"

#include "Cell.h"

namespace mack {

static const int MAZE_WIDTH = 16;
static const int MAZE_HEIGHT = 16;
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

class MackAlgo : IAlgorithm {

public:
    void solve(sim::MouseInterface* mouse);

private:
    sim::MouseInterface* m_mouse;
    Cell m_maze[MAZE_WIDTH][MAZE_HEIGHT];
    int m_x;
    int m_y;
    int m_d;
    bool m_onWayToCenter;

    Cell* getNextMove();
    float getTurnCost();
    float getStraightAwayCost(int length);
    bool inspectNeighbor(Cell* current, Cell* neighbor, int direction);

    void initializeDestinationDistance();
    Cell* getClosestDestinationCell();
    Cell* min(Cell* one, Cell* two);

    void readWalls();
    bool inGoal(int x, int y);

    void moveForward();
    void turnLeft();
    void turnRight();
    void turnAround();

    Cell* getFrontCell();
    Cell* getLeftCell();
    Cell* getRightCell();
    Cell* getRearCell();
    bool spaceFront();
    bool spaceLeft();
    bool spaceRight();

    bool isOneCellAway(Cell* target);
    void moveOneCell(Cell* target);

    void setColor(int x, int y, char color);
    void resetColors();
    void colorCenter(char color);

};

} // namespace mack
