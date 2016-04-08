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

    // Assert that the maze size is sane
    ASSERT_LE(1, Maze::WIDTH);
    ASSERT_LE(1, Maze::HEIGHT);
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
            if (x == 0) { 
                setCellWall(Maze::getIndex(x, y), Direction::WEST, true);
            }
            if (y == 0) {
                setCellWall(Maze::getIndex(x, y), Direction::SOUTH, true);
            }
            if (x == Maze::WIDTH - 1) {
                setCellWall(Maze::getIndex(x, y), Direction::EAST, true);
            }
            if (y == Maze::HEIGHT - 1) {
                setCellWall(Maze::getIndex(x, y), Direction::NORTH, true);
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

        // Read the walls if not known
        readWalls();

        // Move as far as possible
        if (!move()) {
            // If we've detected an unsolvable maze, just die
            m_mouse->warn("Unsolvable maze detected. I'm giving up...");
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
    static byte sequenceNumber = 0;
    sequenceNumber += 1;

    // TODO: MACK - convince myself we won't run out
    // m_mouse->info(std::to_string(sequenceNumber));

    // Initialize the source cell
    byte sourceMazeIndex = Maze::getIndex(m_x, m_y);
    Maze::info[sourceMazeIndex].sequenceNumber = sequenceNumber;
    Maze::info[sourceMazeIndex].parentIndex = sourceMazeIndex;
    Maze::info[sourceMazeIndex].sourceDirection = m_d;
    // Purposefully don't set straightAwayLength
    setCellDistance(sourceMazeIndex, 0);

    // TODO:
    resetDestinationCellDistances();

    // Clear all tile color
    m_mouse->clearAllTileColor();

    Heap heap;
    heap.push(sourceMazeIndex);
    int maxSize = 1;
    while (!heap.empty()) {

        byte currentMazeIndex = heap.pop();
        byte x = Maze::getX(currentMazeIndex);
        byte y = Maze::getY(currentMazeIndex);

        // TODO: MACK - make these an option
        if (true) {
            if (true) {
                m_mouse->delay(10);
            }
            colorTile(x, y, 'Y');
            // We needn't explore any further
            if (currentMazeIndex == getClosestDestinationCell()) {
                break;
            }
        }

        // Inspect neighbors if they're not yet visited
        // NOTE: Inspecting and examining are not the same thing!
        for (byte direction = 0; direction < 4; direction += 1) {
            if (!Maze::isWall(currentMazeIndex, direction)) {
                byte neighborMazeIndex = getNeighboringCell(Maze::getX(currentMazeIndex), Maze::getY(currentMazeIndex), direction);
                checkNeighbor(currentMazeIndex, neighborMazeIndex, direction, &heap);
            }
        }
        if (heap.size() > maxSize) {
            maxSize = heap.size();
        }
    }
    // TODO: MACK
    //m_mouse->info(std::to_string(maxSize));

    colorCenter('G');

    // If there's no path to the destination, the maze is unsolvable
    if (Maze::info[getClosestDestinationCell()].parentIndex == getClosestDestinationCell()) {
        return false;
    }

    // WARNING: Ugly hack to reverse the "list" of parents to a "list" of children.
    // We need this so that we can buffer the moves for the drive code.

    byte next = getClosestDestinationCell();
    colorTile(Maze::getX(next), Maze::getY(next), 'B');
    byte current = Maze::info[next].parentIndex;
    byte prev = Maze::info[current].parentIndex;
    Maze::info[next].parentIndex = next;

    while (prev != current) {
        colorTile(Maze::getX(current), Maze::getY(current), 'B');
        Maze::info[current].parentIndex = next;
        next = current;
        current = prev;
        prev = Maze::info[current].parentIndex;
    }
    Maze::info[current].parentIndex = next;

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

    while (current != Maze::info[current].parentIndex && Maze::isKnown(current, Maze::info[next].sourceDirection)) {
        moveOneCell(next);
        current = next;
        next = Maze::info[next].parentIndex;
    }

    // Successful move
    return true;
}

float MackAlgoTwo::getTurnCost() {
    return 1.0;
}

float MackAlgoTwo::getStraightAwayCost(byte length) {
    return 1.0 / length;
}

void MackAlgoTwo::checkNeighbor(
        byte current,
        byte neighbor,
        byte direction,
        Heap* heap) {

    // Determine the cost if routed through the currect node
    float costToNeighbor = Maze::info[current].distance;
    if (Maze::info[current].sourceDirection == direction) {
        costToNeighbor += getStraightAwayCost(Maze::info[current].straightAwayLength + 1);
    }
    else {
        costToNeighbor += getTurnCost();
    }

    // Make updates to the neighbor node if necessary
    if (Maze::info[neighbor].sequenceNumber != Maze::info[current].sequenceNumber || costToNeighbor < Maze::info[neighbor].distance) {
        bool pushToHeap = false;
        if (Maze::info[neighbor].sequenceNumber != Maze::info[current].sequenceNumber) {
            Maze::info[neighbor].sequenceNumber = Maze::info[current].sequenceNumber;
            pushToHeap = true;
        }
        Maze::info[neighbor].parentIndex = current;
        setCellDistance(neighbor, costToNeighbor);
        Maze::info[neighbor].sourceDirection = direction;
        if (Maze::info[current].sourceDirection == direction) {
            Maze::info[neighbor].straightAwayLength = Maze::info[current].straightAwayLength + 1;
        }
        else {
            Maze::info[neighbor].straightAwayLength = 1;
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
                setCellDistance(Maze::getIndex(x, y), maxDistance);
            }
        }
    }
    else {
        setCellDistance(Maze::getIndex(0, 0), maxDistance);
    }
}

byte MackAlgoTwo::cellMin(byte one, byte two) {
    if (one == 255) {
        return two;
    }
    return (Maze::info[one].distance < Maze::info[two].distance ? one : two);
}

byte MackAlgoTwo::getClosestDestinationCell() {
    byte closest = 255;
    if (m_onWayToCenter) {
        for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
            for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
                closest = cellMin(closest, Maze::getIndex(x, y));
            }
        }
    }
    else {
        closest = Maze::getIndex(0, 0);
    }
    return closest;
}

bool MackAlgoTwo::hasNeighboringCell(
        byte x,
        byte y,
        byte direction) {

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

    return neighboringCellExists;
}

byte MackAlgoTwo::getNeighboringCell(
        byte x,
        byte y,
        byte direction) {

    ASSERT_TR(hasNeighboringCell(x, y, direction));

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

    return Maze::getIndex(x, y);
}

bool MackAlgoTwo::isOneCellAway(byte target) {

    byte x = Maze::getX(target);
    byte y = Maze::getY(target);
    
    if ((m_x == x) && (m_y + 1 == y) && !Maze::isWall(m_x, m_y, Direction::NORTH)) {
        return true;
    }
    else if ((m_x == x) && (m_y - 1 == y) && !Maze::isWall(m_x, m_y, Direction::SOUTH)) {
        return true;
    }
    else if ((m_x + 1 == x) && (m_y == y) && !Maze::isWall(m_x, m_y, Direction::EAST)) {
        return true;
    }
    else if ((m_x - 1 == x) && (m_y == y) && !Maze::isWall(m_x, m_y, Direction::WEST)) {
        return true;
    }
    
    return false;
}

void MackAlgoTwo::moveOneCell(byte target) {

    ASSERT_TR(isOneCellAway(target));

    byte x = Maze::getX(target);
    byte y = Maze::getY(target);
    
    byte moveDirection = Direction::NORTH;
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
        byte direction = (m_d + i + 4) % 4;

        // If the wall is not already known
        if (!Maze::isKnown(m_x, m_y, direction)) {

            // Read and update the wall value
            bool isWall = readWall(direction);
            setCellWall(Maze::getIndex(m_x, m_y), direction, isWall);

            // If a neighboring cell exists, set the neighbor's wall too
            if (hasNeighboringCell(m_x, m_y, direction)) {
                byte neighboringCell = getNeighboringCell(m_x, m_y, direction);
                byte oppositeDirection= (direction + 2) % 4;
                setCellWall(neighboringCell, oppositeDirection, isWall);
            }
        }
    }
}

bool MackAlgoTwo::readWall(byte direction) {
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

bool MackAlgoTwo::inGoal(byte x, byte y) {
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

void MackAlgoTwo::colorTile(byte x, byte y, char color) {
    m_mouse->setTileColor(x, y, color);
}

void MackAlgoTwo::setCellDistance(byte mazeIndex, float distance) {
    Maze::info[mazeIndex].distance = distance;
    m_mouse->setTileText(Maze::getX(mazeIndex), Maze::getY(mazeIndex), std::to_string(distance));
}

void MackAlgoTwo::setCellWall(byte cell, byte direction, bool isWall) {
    Maze::setWall(cell, direction, isWall);
    static char directionChars[] = {'n', 'e', 's', 'w'};
    m_mouse->declareWall(Maze::getX(cell), Maze::getY(cell), directionChars[direction], isWall);
}

} // namespace mackAlgoTwo
