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
    Cell m_maze[Maze::WIDTH][Maze::HEIGHT]; // TODO: MACK
    Info m_info[Maze::WIDTH][Maze::HEIGHT]; // TODO: MACK
    byte m_x;
    byte m_y;
    byte m_d;
    bool m_onWayToCenter;
    byte m_moveBufferIndex;

    bool move();
    float getTurnCost();
    float getStraightAwayCost(byte length);
    void checkNeighbor(
        Cell* current,
        Cell* neighbor,
        byte direction,
        CellHeap* heap);

    void resetDestinationCellDistances();
    Cell* cellMin(Cell* one, Cell* two);
    Cell* getClosestDestinationCell();
    Cell* getNeighboringCell(
        byte x,
        byte y,
        byte direction);

    bool isOneCellAway(Cell* target);
    void moveOneCell(Cell* target);

    void readWalls();
    bool readWall(byte direction);
    bool inGoal(byte x, byte y);

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
    void colorTile(byte x, byte y, char color);

    // TODO: MACK - we don't have mouseInterface in cell.h, so we need helpers here
    void setCellDistance(Cell* cell, float distance);
    void setCellWall(Cell* cell, byte direction, bool isWall);
};

} // namespace mackAlgoTwo
