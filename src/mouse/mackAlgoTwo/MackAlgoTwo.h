#pragma once

#include "../IMouseAlgorithm.h"
#include "Cell.h"
#include "CellHeap.h"
#include "Center.h"
#include "Direction.h"

namespace mackAlgoTwo {

static const unsigned char MAZE_WIDTH = 16;  // XXX
static const unsigned char MAZE_HEIGHT = 16; // XXX

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
    unsigned char m_x;
    unsigned char m_y;
    Direction m_d;
    bool m_onWayToCenter;
    unsigned char m_moveBufferIndex;

    bool move();
    float getTurnCost();
    float getStraightAwayCost(unsigned char length);
    void checkNeighbor(
        Cell* current,
        Cell* neighbor,
        Direction direction,
        CellHeap* heap);

    Center getCenter();
    void resetDestinationCellDistances();
    Cell* cellMin(Cell* one, Cell* two);
    Cell* getClosestDestinationCell();
    Cell* getNeighboringCell(
        unsigned char x,
        unsigned char y,
        Direction direction);

    bool isOneCellAway(Cell* target);
    void moveOneCell(Cell* target);

    void readWalls();
    bool readWall(Direction direction);
    bool inGoal(unsigned char x, unsigned char y);

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

    void setColor(unsigned char x, unsigned char y, char color);
    void resetColors();
    void colorCenter(char color);

    // TODO: MACK - we don't have mouseInterface in cell.h, so we need helpers here
    void setCellDistance(Cell* cell, float distance);
    void setCellWall(Cell* cell, Direction direction, bool isWall);
};

} // namespace mackAlgoTwo
