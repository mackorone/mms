#pragma once

#include "Byte.h"
#include "Direction.h"
#include "Heap.h"
#include "Interface.h"
#include "Maze.h"
#include "Options.h"

class Algo {

public:

    void solve(Interface* interface);

private:

    static const bool FAST_STRAIGHT_AWAYS = true;

    Interface* m_mouse;

    byte m_x; // X position of the mouse
    byte m_y; // Y position of the mouse
    byte m_d; // Direction of the mouse
    byte m_mode; // Modus operandi of the mouse
    byte m_initialDirection; // As the name states

    bool shouldColorVisitedCells() const;
    byte colorVisitedCellsDelayMs() const;

    bool resetButtonPressed();
    void acknowledgeResetButtonPressed();

    twobyte getTurnCost();
    twobyte getStraightAwayCost(byte length);

    void reset();
    void step();

    byte generatePath(byte start);
    void drawPath(byte start);
    void followPath(byte start);
    byte getFirstUnknown(byte start);

    void checkNeighbor(byte cell, byte direction);
    byte reverseLinkedList(byte cell);

    bool inCenter(byte x, byte y);
    bool inOrigin(byte x, byte y);

    void colorCenter(char color);
    void resetDestinationCellDistances();
    byte getClosestDestinationCell();

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
