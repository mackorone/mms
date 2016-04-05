#pragma once

#include "../IMouseAlgorithm.h"
#include "Cell.h"
#include "CellHeap.h"

namespace mackAlgoTwo {

static const int MAZE_WIDTH = 16;  // XXX
static const int MAZE_HEIGHT = 16; // XXX
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

class MackAlgoTwo : public IMouseAlgorithm {

public:
    int tileTextNumberOfRows() const;
    int tileTextNumberOfCols() const;
    bool useTileEdgeMovements() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

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
    bool inspectNeighbor(Cell* current, Cell* neighbor, int direction, CellHeap* heap);

    void initializeDestinationDistance();
    Cell* getClosestDestinationCell();
    Cell* cellMin(Cell* one, Cell* two);

    void readWalls();
    bool inGoal(int x, int y);

    void turnLeftUpdateState();
    void turnRightUpdateState();
    void turnAroundUpdateState();
    void moveForwardUpdateState();

    void turnLeft();
    void turnRight();
    void turnAround();

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

    void setColor(int x, int y, char color);
    void resetColors();
    void colorCenter(char color);

};

} // namespace mackAlgoTwo
