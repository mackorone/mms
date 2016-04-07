#pragma once

#include "../IMouseAlgorithm.h"
#include "Byte.h"
#include "Cell.h"
#include "CellHeap.h"
#include "Direction.h"
#include "Maze.h"

namespace mackAlgoTwo {

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
    Cell m_maze[Maze::WIDTH][Maze::HEIGHT];
    unsigned char m_x;
    unsigned char m_y;
    unsigned char m_d;
    bool m_onWayToCenter;
    unsigned char m_moveBufferIndex;

    bool move();
    float getTurnCost();
    float getStraightAwayCost(unsigned char length);
    void checkNeighbor(
        Cell* current,
        Cell* neighbor,
        unsigned char direction,
        CellHeap* heap);

    void resetDestinationCellDistances();
    Cell* cellMin(Cell* one, Cell* two);
    Cell* getClosestDestinationCell();
    Cell* getNeighboringCell(
        unsigned char x,
        unsigned char y,
        unsigned char direction);

    bool isOneCellAway(Cell* target);
    void moveOneCell(Cell* target);

    void readWalls();
    bool readWall(unsigned char direction);
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

    void colorCenter(char color);
    void colorTile(unsigned char x, unsigned char y, char color);

    // TODO: MACK - we don't have mouseInterface in cell.h, so we need helpers here
    void setCellDistance(Cell* cell, float distance);
    void setCellWall(Cell* cell, unsigned char direction, bool isWall);
};

} // namespace mackAlgoTwo
