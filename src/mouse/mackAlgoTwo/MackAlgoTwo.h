#pragma once

#include "../IMouseAlgorithm.h"
#include "Byte.h"
#include "Direction.h"
#include "Heap.h"
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
    byte m_x;
    byte m_y;
    byte m_d;
    bool m_onWayToCenter;
    byte m_moveBufferIndex;

    bool move();
    float getTurnCost();
    float getStraightAwayCost(byte length);
    void checkNeighbor(
        byte current,
        byte neighbor,
        byte direction);

    void resetDestinationCellDistances();
    byte cellMin(byte one, byte two);
    byte getClosestDestinationCell();
    bool hasNeighboringCell(byte x, byte y, byte direction);
    byte getNeighboringCell(byte x, byte y, byte direction);

    bool isOneCellAway(byte target);
    void moveOneCell(byte target);

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
    void colorTile(byte cell, char color);

    // TODO: MACK - we don't have mouseInterface in cell.h, so we need helpers here
    void setCellDistance(byte mazeIndex, float distance);
    void setCellWall(byte cell, byte direction, bool isWall);
};

} // namespace mackAlgoTwo
