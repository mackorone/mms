#pragma once

#include "../IMouseAlgorithm.h"
#include "Byte.h"
#include "Direction.h"
#include "Heap.h"
#include "Maze.h"

namespace mackAlgoTwo {

class MackAlgoTwo : public IMouseAlgorithm {

public:
    std::string initialDirection() const;
    int tileTextNumberOfRows() const;
    int tileTextNumberOfCols() const;
    bool useTileEdgeMovements() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    sim::MouseInterface* m_mouse;
    byte m_x; // X position of the mouse
    byte m_y; // Y position of the mouse
    byte m_d; // Direction of the mouse
    byte m_mode; // Modus operandi of the mouse

    bool shouldColorVisitedCells() const;
    byte colorVisitedCellsDelayMs() const;

    bool resetButtonPressed();
    void acknowledgeResetButtonPressed();

    twobyte getTurnCost();
    twobyte getStraightAwayCost(byte length);

    void reset();

    void getToCenterStep();
    void exploreStep();
    void returnToOriginStep();
    void solveStep();

    byte generatePath(byte start, byte destination = 0, bool reversed = false);
    void drawPath(byte start);
    void followPath(byte start);

    void checkNeighbor(byte cell, byte direction);
    byte reverseLinkedList(byte cell);

    bool inCenter();
    void colorCenter(char color);
    void resetDestinationCellDistances(byte destination = 0);
    byte getClosestDestinationCell(byte destination = 0);

    byte getOppositeDirection(byte direction);
    bool hasNeighboringCell(byte cell, byte direction);
    byte getNeighboringCell(byte cell, byte direction);

    bool isOneCellAway(byte target);
    void moveOneCell(byte target);

    void readWalls();
    bool readWall(byte direction);

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

    void setCellDistance(byte cell, twobyte distance);
    void setCellWall(byte cell, byte direction, bool isWall, bool bothSides = true);
    void unsetCellWall(byte cell, byte direction, bool bothSides = true);

};

} // namespace mackAlgoTwo
