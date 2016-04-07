#include "MackAlgoTwo.h"

#include <limits>

#include "Maze.h"

namespace mackAlgoTwo {

int MackAlgoTwo::tileTextNumberOfRows() const {
    return 1;
}

int MackAlgoTwo::tileTextNumberOfCols() const {
    return 5;
}

bool MackAlgoTwo::useTileEdgeMovements() const {
    // TODO: MACK
    return false;
}

void MackAlgoTwo::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {

    // Initialize the MouseInterface pointer
    m_mouse = mouse;

    // Assert our maze size is sane
    ASSERT_LT(0, Maze::WIDTH);
    ASSERT_LT(0, Maze::HEIGHT);
    ASSERT_LE(Maze::WIDTH, 16);
    ASSERT_LE(Maze::HEIGHT, 16);
    ASSERT_EQ(Maze::WIDTH, mazeWidth);
    ASSERT_EQ(Maze::HEIGHT, mazeHeight);

    // TODO: MACK
    if ('n' != initialDirection) {
        m_mouse->warn("Incompatible initial direction. I'm giving up.");
        return;
    }

    // Initialize the (perimeter of the) maze
    for (byte x = 0; x < Maze::WIDTH; x += 1) {
        for (byte y = 0; y < Maze::HEIGHT; y += 1) {
            // TODO: MACK - kill this
            m_maze[x][y].init(x, y, Maze::WIDTH, Maze::HEIGHT);
            if (x == 0) { 
                Maze::setWall(x, y, Direction::WEST, true);
            }
            if (y == 0) {
                Maze::setWall(x, y, Direction::SOUTH, true);
            }
            if (x == Maze::WIDTH - 1) {
                Maze::setWall(x, y, Direction::EAST, true);
            }
            if (y == Maze::HEIGHT - 1) {
                Maze::setWall(x, y, Direction::NORTH, true);
            }
        }
    }

    // Initialize the mouse
    m_x = 0;
    m_y = 0;
    m_d = Direction::NORTH;
    m_onWayToCenter = true;

    // Go the center, the the start, forever
    while (true) {

/*
#if (!SIMULATOR)
        while (!movesDoneAndWallsSet) {
            // Wait for the walls to be ready
        }
#endif
*/

        // Read the walls if not known
        readWalls();

        // Move as far as possible
        if (!move()) {
            // If we've detected an unsolvable maze, just die
            return;
        }

        // Change the destination
        if (m_onWayToCenter ? inGoal(m_x, m_y) : m_x == 0 && m_y == 0) {
            m_onWayToCenter = !m_onWayToCenter;
        }
    }
}

bool MackAlgoTwo::move() {

    // Use Dijkstra's algo to determine the next best move

    // Increment the sequence number so that we know that old cell data is stale
    static unsigned char sequenceNumber = 0;
    sequenceNumber += 1;

    // TODO: MACK - convince myself we won't run out
    // m_mouse->info(std::to_string(sequenceNumber));

    // Initialize the source cell
    Cell* source = &m_maze[m_x][m_y];
    source->info.sequenceNumber = sequenceNumber;
    source->info.parent = NULL;
    source->info.parentPosition = source->getPosition();
    source->info.sourceDirection = m_d;
    // Purposefully don't set straightAwayLength
    setCellDistance(source, 0);

    // TODO:
    resetDestinationCellDistances();

    // Clear all tile color
    m_mouse->clearAllTileColor();

    CellHeap heap;
    heap.push(source);
    int maxSize = 1;
    while (!heap.empty()) {

        Cell* current = heap.pop();
        unsigned char x = current->getX();
        unsigned char y = current->getY();

        // TODO: MACK - why don't we short circuit the first time around?
        // I think I need to reset the dest distances

        // TODO: MACK - make this an option
        if (false) {
            // TODO: MACK - make this an option
            if (true) {
                m_mouse->delay(10);
            }
            colorTile(x, y, 'Y');
            // We needn't explore any further
            if (current == getClosestDestinationCell()) {
                break;
            }
        }

        // Inspect neighbors if they're not yet visited
        // NOTE: Inspecting and examining are not the same thing!
        for (unsigned char direction = 0; direction < 4; direction += 1) {
            if (!current->isWall(direction)) {
                Cell* neighbor = getNeighboringCell(current->getX(), current->getY(), direction);
                checkNeighbor(current, neighbor, direction, &heap);
            }
        }
        if (heap.size() > maxSize) {
            maxSize = heap.size();
        }
    }
    // TODO: MACK
    m_mouse->info(std::to_string(maxSize));

    colorCenter('G');

    // If there's no path to the destination, the maze is unsolvable
    // TODO: MACK - parent position here? parent is the own position
    if (getClosestDestinationCell()->info.parent == NULL) {
        m_mouse->warn("Unsolvable maze detected. I'm giving up...");
        return false;
    }

    // WARNING: Ugly hack to reverse the "list" of parents to a "list" of children.
    // We need this so that we can buffer the moves for the drive code.

    Cell* next = getClosestDestinationCell();
    colorTile(next->getX(), next->getY(), 'B');

    // TODO: MACK
    Cell* current = &m_maze[Maze::getX(next->info.parentPosition)][Maze::getY(next->info.parentPosition)];
    Cell* prev = &m_maze[Maze::getX(current->info.parentPosition)][Maze::getY(current->info.parentPosition)];

    next->info.parent = NULL;
    next->info.parentPosition = next->getPosition();

    while (prev != current) {
        colorTile(current->getX(), current->getY(), 'B');
        current->info.parent = next;
        current->info.parentPosition = next->getPosition();
        next = current;
        current = prev;
        prev = &m_maze[Maze::getX(current->info.parentPosition)][Maze::getY(current->info.parentPosition)];
    }

    // Displays the color buffer
    /*
    Cell* colorCurrent = current;
    Cell* colorNext = next;
    colorTile(colorCurrent->getX(), colorCurrent->getY(), 'V');
    while (colorNext != NULL && colorCurrent->isKnown(colorNext->info.sourceDirection)) {
        colorTile(colorNext->getX(), colorNext->getY(), 'V');
        colorCurrent = colorNext;
        colorNext = colorNext->info.parent;
    }

    // First, make sure that we reset the move buffer index
    m_moveBufferIndex = 0;
    */

    // WARNING: As a result of the ugly hack to reverse the list, we have to use
    // the parent field of the cells, though its really a child pointer at this point

    // TODO: MACK - this isn't quite right...
    //while (next->getPosition() != next->info.parentPosition && current->isKnown(next->info.sourceDirection)) {
    while (next != NULL && current->isKnown(next->info.sourceDirection)) {
        moveOneCell(next);
        current = next;
        // TODO: MACK - this isn't quite right...
        //next = &m_maze[Maze::getX(next->info.parentPosition)][Maze::getY(next->info.parentPosition)];
        next = next->info.parent;
    }

/*
    // Terminate the buffer index and tell the control code the moves are ready
#if (!SIMULATOR)
    movesBuffer[m_moveBufferIndex] = '\0';
    movesReady = true;
#endif
*/

    // Successful move
    return true;
}

float MackAlgoTwo::getTurnCost() {
    return 1.0;
}

float MackAlgoTwo::getStraightAwayCost(unsigned char length) {
    return 1.0 / length;
}

void MackAlgoTwo::checkNeighbor(
        Cell* current,
        Cell* neighbor,
        unsigned char direction,
        CellHeap* heap) {

    // Determine the cost if routed through the currect node
    float costToNeighbor = current->info.distance;
    if (current->info.sourceDirection == direction) {
        costToNeighbor += getStraightAwayCost(current->info.straightAwayLength + 1);
    }
    else {
        costToNeighbor += getTurnCost();
    }

    // Make updates to the neighbor node if necessary
    if (neighbor->info.sequenceNumber != current->info.sequenceNumber || costToNeighbor < neighbor->info.distance) {
        bool pushToHeap = false;
        if (neighbor->info.sequenceNumber != current->info.sequenceNumber) {
            neighbor->info.sequenceNumber = current->info.sequenceNumber;
            pushToHeap = true;
        }
        neighbor->info.parent = current;
        neighbor->info.parentPosition = current->getPosition();
        setCellDistance(neighbor, costToNeighbor);
        neighbor->info.sourceDirection = direction;
        if (current->info.sourceDirection == direction) {
            neighbor->info.straightAwayLength = current->info.straightAwayLength + 1;
        }
        else {
            neighbor->info.straightAwayLength = 1;
        }
        if (pushToHeap) {
            heap->push(neighbor);
        }
        else {
            heap->update(neighbor);
        }
    }
}

void MackAlgoTwo::resetDestinationCellDistances() {
    static float maxDistance = std::numeric_limits<float>::max();
    if (m_onWayToCenter) {
        for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
            for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
                setCellDistance(&m_maze[x][y], maxDistance);
            }
        }
    }
    else {
        setCellDistance(&m_maze[0][0], maxDistance);
    }
}

Cell* MackAlgoTwo::cellMin(Cell* one, Cell* two) {
    if (one == NULL) {
        return two;
    }
    return (one->info.distance < two->info.distance ? one : two);
}

Cell* MackAlgoTwo::getClosestDestinationCell() {
    Cell* closest = NULL;
    if (m_onWayToCenter) {
        for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
            for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
                closest = cellMin(closest, &m_maze[x][y]);
            }
        }
    }
    else {
        closest = &m_maze[0][0];
    }
    return closest;
}

Cell* MackAlgoTwo::getNeighboringCell(
        unsigned char x,
        unsigned char y,
        unsigned char direction) {

    switch (direction) {
        case Direction::NORTH:
            y += 1;
            break;
        case Direction::EAST:
            x += 1;
            break;
        case Direction::SOUTH:
            y -= 1;
            break;
        case Direction::WEST:
            x -= 1;
            break;
    }

    bool neighboringCellExists = (
        0 <= x &&
        0 <= y &&
        x < Maze::WIDTH &&
        y < Maze::HEIGHT
    );

    return (neighboringCellExists ? &m_maze[x][y] : nullptr);
}

bool MackAlgoTwo::isOneCellAway(Cell* target) {

    unsigned char x = target->getX();
    unsigned char y = target->getY();
    
    if ((m_x == x) && (m_y + 1 == y) && !m_maze[m_x][m_y].isWall(Direction::NORTH)) {
        return true;
    }
    else if ((m_x == x) && (m_y - 1 == y) && !m_maze[m_x][m_y].isWall(Direction::SOUTH)) {
        return true;
    }
    else if ((m_x + 1 == x) && (m_y == y) && !m_maze[m_x][m_y].isWall(Direction::EAST)) {
        return true;
    }
    else if ((m_x - 1 == x) && (m_y == y) && !m_maze[m_x][m_y].isWall(Direction::WEST)) {
        return true;
    }
    
    return false;
}

void MackAlgoTwo::moveOneCell(Cell* target) {

    ASSERT_TR(isOneCellAway(target));

    unsigned char x = target->getX();
    unsigned char y = target->getY();
    
    unsigned char moveDirection = Direction::NORTH;
    if (x > m_x) {
        moveDirection = Direction::EAST;
    }
    else if (y < m_y) {
        moveDirection = Direction::SOUTH;
    }
    else if (x < m_x) {
        moveDirection = Direction::WEST;
    }

    if (moveDirection == m_d) {
        moveForward();
    }
    else if (moveDirection == (m_d + 1) % 4) {
        rightAndForward();
    }
    else if (moveDirection == (m_d + 2) % 4) {
        aroundAndForward();
    }
    else if (moveDirection == (m_d + 3) % 4) {
        leftAndForward();
    }
}

void MackAlgoTwo::readWalls() {

    // For each of [left, front, right]
    for (int i = -1; i <= 1; i += 1) {
        unsigned char direction = (m_d + i + 4) % 4;

        // If the wall is not already known
        if (!m_maze[m_x][m_y].isKnown(direction)) {

            // Read and update the wall value
            bool isWall = readWall(direction);
            setCellWall(&m_maze[m_x][m_y], direction, isWall);

            // If a neighboring cell exists, set the neighbor's wall too
            Cell* neighboringCell = getNeighboringCell(m_x, m_y, direction);
            if (neighboringCell != nullptr) {
                unsigned char oppositeDirection= (direction + 2) % 4;
                setCellWall(neighboringCell, oppositeDirection, isWall);
            }
        }
    }
}

bool MackAlgoTwo::readWall(unsigned char direction) {
    switch ((direction - m_d + 4) % 4) {
        case 0:
            return m_mouse->wallFront();
        case 1:
            return m_mouse->wallRight();
        case 3:
            return m_mouse->wallLeft();
    }
    // We should never get here
    ASSERT_TR(false);
}

bool MackAlgoTwo::inGoal(unsigned char x, unsigned char y) {
    for (byte x2 = Maze::CLLX; x2 <= Maze::CURX; x2 += 1) {
        for (byte y2 = Maze::CLLY; y2 <= Maze::CURY; y2 += 1) {
            if (x == x2 && y == y2) {
                return true;
            }
        }
    }
    return false;
}

void MackAlgoTwo::turnLeftUpdateState() {
    m_d = (m_d + 3) % 4;
}

void MackAlgoTwo::turnRightUpdateState() {
    m_d = (m_d + 1) % 4;
}

void MackAlgoTwo::turnAroundUpdateState() {
    m_d = (m_d + 2) % 4;
}

void MackAlgoTwo::moveForwardUpdateState() {
    m_x += (m_d == Direction::EAST  ? 1 : (m_d == Direction::WEST  ? -1 : 0));
    m_y += (m_d == Direction::NORTH ? 1 : (m_d == Direction::SOUTH ? -1 : 0));
}

void MackAlgoTwo::turnLeft() {
    turnLeftUpdateState();
    m_mouse->turnLeft();
}

void MackAlgoTwo::turnRight() {
    turnRightUpdateState();
    m_mouse->turnRight();
}

void MackAlgoTwo::turnAround() {
    turnAroundUpdateState();
    m_mouse->turnAroundLeft();
}

void MackAlgoTwo::moveForward() {
    moveForwardUpdateState();
    m_mouse->moveForward();
}

void MackAlgoTwo::leftAndForward() {
    turnLeft();
    moveForward();
}

void MackAlgoTwo::rightAndForward() {
    turnRight();
    moveForward();
}

void MackAlgoTwo::aroundAndForward() {
    turnAround();
    moveForward();
}

void MackAlgoTwo::colorCenter(char color) {
    for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
        for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
            colorTile(x, y, color);
        }
    }
}

void MackAlgoTwo::colorTile(unsigned char x, unsigned char y, char color) {
    m_mouse->setTileColor(x, y, color);
}

void MackAlgoTwo::setCellDistance(Cell* cell, float distance) {
    cell->info.distance = distance;
    m_mouse->setTileText(cell->getX(), cell->getY(), std::to_string(distance));
}

void MackAlgoTwo::setCellWall(Cell* cell, unsigned char direction, bool isWall) {
    cell->setWall(direction, isWall);
    static char directionChars[] = {'n', 'e', 's', 'w'};
    m_mouse->declareWall(cell->getX(), cell->getY(), directionChars[direction], isWall);
}

} // namespace mackAlgoTwo
