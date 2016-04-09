#include "MackAlgoTwo.h"

#include <limits>

#include "Maze.h"

namespace mackAlgoTwo {

std::string MackAlgoTwo::initialDirection() const {
    return "OPENING";
}

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
    switch (initialDirection) {
        case 'n':
            m_d = Direction::NORTH;
            break;
        case 'e':
            m_d = Direction::EAST;
            break;
        default:
            m_mouse->warn(
                "Cannot start facing south or west. I'm giving up...");
            return;
    }
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
        if (m_onWayToCenter ? inCenter() : m_x == 0 && m_y == 0) {
            m_onWayToCenter = !m_onWayToCenter;
        }
    }
}

// TODO: MACK - probably don't need to be float here
float MackAlgoTwo::getTurnCost() {
    return 1.0;
}

float MackAlgoTwo::getStraightAwayCost(byte length) {
    return 1.0 / length;
}

bool MackAlgoTwo::inCenter() {
    for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
        for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
            if (m_x == x && m_y == y) {
                return true;
            }
        }
    }
    return false;
}

bool MackAlgoTwo::move() {

    // TODO: MACK - 
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
    // Purposefully don't (re)set straightAwayLength
    setCellDistance(sourceMazeIndex, 0);

    // Set the destination tiles to have max distance so that
    // getClosestDestinationCell() isn't dependent on previous iterations'
    // distance values
    resetDestinationCellDistances();

    // Clear all tile color
    m_mouse->clearAllTileColor();

    // Dijkstra's algo
    Heap::push(sourceMazeIndex);
    while (0 < Heap::size()) {
        byte cell = Heap::pop();
        for (byte direction = 0; direction < 4; direction += 1) {
            if (!Maze::isWall(cell, direction)) {
                checkNeighbor(cell, direction);
            }
        }
        if (false) {
            m_mouse->delay(10);
            colorCell(cell, 'Y');
        }
        // TODO: MACK - this makes it hard for the sequence number situation :/
        if (cell == getClosestDestinationCell()) {
            while (0 < Heap::size()) {
                Heap::pop();
            }
            break;
        }
    }

    // TODO: MACK
    colorCenter('G');

    // If there's no path to the destination, the maze is unsolvable
    if (Maze::info[getClosestDestinationCell()].parentIndex == getClosestDestinationCell()) {
        return false;
    }

    // WARNING: Ugly hack to reverse the "list" of parents to a "list" of children.
    // We need this so that we can buffer the moves for the drive code.

    byte next = getClosestDestinationCell();
    colorCell(next, 'B');
    byte current = Maze::info[next].parentIndex;
    byte prev = Maze::info[current].parentIndex;
    Maze::info[next].parentIndex = next;

    while (prev != current) {
        colorCell(current, 'B');
        Maze::info[current].parentIndex = next;
        next = current;
        current = prev;
        prev = Maze::info[current].parentIndex;
    }
    Maze::info[current].parentIndex = next;

    // Displays the color buffer
    byte colorCurrent = current;
    byte colorNext = next;
    colorCell(colorCurrent, 'V');
    while (Maze::info[colorCurrent].parentIndex != colorCurrent && Maze::isKnown(colorCurrent, Maze::info[colorNext].sourceDirection)) {
        colorCell(colorNext, 'V');
        colorCurrent = colorNext;
        colorNext = Maze::info[colorNext].parentIndex;
    }

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

void MackAlgoTwo::checkNeighbor(byte cell, byte direction) {
    byte neighbor = getNeighboringCell(cell, direction);

    // Determine the cost if routed through the currect node
    float costToNeighbor = Maze::info[cell].distance;
    if (Maze::info[cell].sourceDirection == direction) {
        costToNeighbor += getStraightAwayCost(Maze::info[cell].straightAwayLength + 1);
    }
    else {
        costToNeighbor += getTurnCost();
    }

    // Make updates to the neighbor node if necessary
    if (Maze::info[neighbor].sequenceNumber != Maze::info[cell].sequenceNumber || costToNeighbor < Maze::info[neighbor].distance) {
        bool pushToHeap = false;
        if (Maze::info[neighbor].sequenceNumber != Maze::info[cell].sequenceNumber) {
            Maze::info[neighbor].sequenceNumber = Maze::info[cell].sequenceNumber;
            pushToHeap = true;
        }
        Maze::info[neighbor].parentIndex = cell;
        setCellDistance(neighbor, costToNeighbor);
        Maze::info[neighbor].sourceDirection = direction;
        if (Maze::info[cell].sourceDirection == direction) {
            Maze::info[neighbor].straightAwayLength = Maze::info[cell].straightAwayLength + 1;
        }
        else {
            Maze::info[neighbor].straightAwayLength = 1;
        }
        if (pushToHeap) {
            Heap::push(neighbor);
        }
        else {
            Heap::update(neighbor);
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

byte MackAlgoTwo::getClosestDestinationCell() {
    byte closest = Maze::getIndex(Maze::CLLX, Maze::CLLY);
    if (m_onWayToCenter) {
        for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
            for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
                byte other = Maze::getIndex(x, y);
                if (Maze::info[other].distance < Maze::info[closest].distance) {
                    closest = other;
                }
            }
        }
    }
    else {
        closest = Maze::getIndex(0, 0);
    }
    return closest;
}

bool MackAlgoTwo::hasNeighboringCell(byte cell, byte direction) {

    byte x = Maze::getX(cell);
    byte y = Maze::getY(cell);

    switch (direction) {
        case Direction::NORTH:
            return y < Maze::HEIGHT - 1;
        case Direction::EAST:
            return x < Maze::WIDTH - 1;
        case Direction::SOUTH:
            return 0 < y;
        case Direction::WEST:
            return 0 < x;
    }
}

byte MackAlgoTwo::getNeighboringCell(byte cell, byte direction) {

    ASSERT_TR(hasNeighboringCell(cell, direction));

    byte x = Maze::getX(cell);
    byte y = Maze::getY(cell);

    switch (direction) {
        case Direction::NORTH:
            return Maze::getIndex(x, y + 1);
        case Direction::EAST:
            return Maze::getIndex(x + 1, y);
        case Direction::SOUTH:
            return Maze::getIndex(x, y - 1);
        case Direction::WEST:
            return Maze::getIndex(x - 1, y);
    }
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
            if (hasNeighboringCell(Maze::getIndex(m_x, m_y), direction)) {
                byte neighboringCell = getNeighboringCell(Maze::getIndex(m_x, m_y), direction);
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
            colorCell(Maze::getIndex(x, y), color);
        }
    }
}

void MackAlgoTwo::colorCell(byte cell, char color) {
    m_mouse->setTileColor(Maze::getX(cell), Maze::getY(cell), color);
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
