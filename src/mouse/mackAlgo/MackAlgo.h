#pragma once

#include "Cell.h"
#include "Options.h"

#if (SIMULATOR)
#include "../IMouseAlgorithm.h"
#endif

namespace mackAlgo {

static const int MAZE_WIDTH = 16;  // XXX
static const int MAZE_HEIGHT = 16; // XXX
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

#if (SIMULATOR)
class MackAlgo : public IMouseAlgorithm {
#else
class MackAlgo {
#endif

public:

#if (SIMULATOR)
    int tileTextNumberOfRows() const;
    int tileTextNumberOfCols() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);
#else
    void solve();
#endif

private:
    sim::MouseInterface* m_mouse;
    Cell m_maze[MAZE_WIDTH][MAZE_HEIGHT];
    int m_x;
    int m_y;
    int m_d;
    bool m_onWayToCenter;
    int m_moveBufferIndex;

    bool move();
    float getTurnCost();
    float getStraightAwayCost(int length);
    bool inspectNeighbor(Cell* current, Cell* neighbor, int direction);

    void initializeDestinationDistance();
    Cell* getClosestDestinationCell();
    Cell* cellMin(Cell* one, Cell* two);

    void readWalls();
    bool inGoal(int x, int y);

    void turnLeftUpdateState();
    void turnRightUpdateState();
    void turnAroundUpdateState();
    void moveForwardUpdateState();

#if (SIMULATOR)
    void turnLeft();
    void turnRight();
    void turnAround();
#endif

    void moveForward();
    void leftAndForward();
    void rightAndForward();
    void aroundAndForward();

    Cell* getFrontCell();
    Cell* getLeftCell();
    Cell* getRightCell();
    Cell* getRearCell();
    bool spaceFront();
    bool spaceLeft();
    bool spaceRight();

    bool isOneCellAway(Cell* target);
    void moveOneCell(Cell* target);

#if (SIMULATOR)
    void setColor(int x, int y, char color);
    void resetColors();
    void colorCenter(char color);
#endif

};

} // namespace mackAlgo
