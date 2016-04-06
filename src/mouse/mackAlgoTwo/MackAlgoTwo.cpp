#include "MackAlgoTwo.h"

#include <limits>

/*
#if (!SIMULATOR)
extern char movesBuffer[256];
extern bool walls_global[3];
extern volatile bool movesReady;
extern volatile bool movesDoneAndWallsSet;
#endif
*/

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

    // TODO: MACK maze has to be less than or equal to 16 x 16
    // We expect a certain maze size and initial direction
    if (MAZE_WIDTH != mazeWidth || MAZE_HEIGHT != mazeHeight) {
        m_mouse->warn("Incompatible maze sizes. I'm giving up.");
        return;
    }
    if ('n' != initialDirection) {
        m_mouse->warn("Incompatible initial direction. I'm giving up.");
        return;
    }

    // Initialize the (perimeter of the) maze
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            m_maze[x][y].init(x, y, MAZE_WIDTH, MAZE_HEIGHT);
        }
    }

    // Initialize the mouse
    m_x = 0;
    m_y = 0;
    m_d = NORTH;
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

    // Initialize the source cell
    Cell* source = &m_maze[m_x][m_y];
    source->info.sequenceNumber = sequenceNumber;
    source->info.parent = NULL;
    source->info.sourceDirection = m_d;
    // Purposefully don't set straightAwayLength
    setCellDistance(source, 0);

    // TODO:
    resetDestinationCellDistances();

    // Clear all tile color
    m_mouse->clearAllTileColor();

    CellHeap heap;
    heap.push(source);
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
            setColor(x, y, 'Y');
            // We needn't explore any further
            if (current == getClosestDestinationCell()) {
                break;
            }
        }

        // Inspect neighbors if they're not yet visited
        // NOTE: Inspecting and examining are not the same thing!
        for (Direction direction = 0; direction < 4; direction += 1) {
            if (!current->isWall(direction)) {
                Cell* neighbor = getNeighboringCell(current->getX(), current->getY(), direction);
                checkNeighbor(current, neighbor, direction, &heap);
            }
        }
    }

    colorCenter('G');

    // If there's no path to the destination, the maze is unsolvable
    if (getClosestDestinationCell()->info.parent == NULL) {
        m_mouse->warn("Unsolvable maze detected. I'm giving up...");
        return false;
    }

    // WARNING: Ugly hack to reverse the "list" of parents to a "list" of children.
    // We need this so that we can buffer the moves for the drive code.
    Cell* next = getClosestDestinationCell();
    setColor(next->getX(), next->getY(), 'B');
    Cell* current = next->info.parent;
    Cell* prev = current->info.parent;
    next->info.parent = NULL;
    while (prev != NULL) {
        setColor(current->getX(), current->getY(), 'B');
        current->info.parent = next;
        next = current;
        current = prev;
        prev = current->info.parent;
    }

    // Displays the color buffer
    Cell* colorCurrent = current;
    Cell* colorNext = next;
    setColor(colorCurrent->getX(), colorCurrent->getY(), 'V');
    while (colorNext != NULL && colorCurrent->isKnown(colorNext->info.sourceDirection)) {
        setColor(colorNext->getX(), colorNext->getY(), 'V');
        colorCurrent = colorNext;
        colorNext = colorNext->info.parent;
    }

    // First, make sure that we reset the move buffer index
    m_moveBufferIndex = 0;

    // WARNING: As a result of the ugly hack to reverse the list, we have to use
    // the parent field of the cells, though its really a child pointer at this point
    while (next != NULL && current->isKnown(next->info.sourceDirection)) {
        moveOneCell(next);
        current = next;
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
        Direction direction,
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

Center MackAlgoTwo::getCenter() {

    Center center;
    for (int i = 0; i < 4; i += 1) {
        center.cells[i] = nullptr;
    }

            center.cells[0] = &m_maze[(MAZE_WIDTH - 1) / 2][(MAZE_HEIGHT - 1) / 2];
    if (MAZE_WIDTH % 2 == 0) {
            center.cells[1] = &m_maze[ MAZE_WIDTH      / 2][(MAZE_HEIGHT - 1) / 2];
        if (MAZE_HEIGHT % 2 == 0) {
            center.cells[2] = &m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2];
            center.cells[3] = &m_maze[ MAZE_WIDTH      / 2][ MAZE_HEIGHT      / 2];
        }
    }
    else if (MAZE_HEIGHT % 2 == 0) {
            center.cells[2] = &m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2];
    }

    return center;
}

void MackAlgoTwo::resetDestinationCellDistances() {
    float maxDistance = std::numeric_limits<float>::max();
    if (m_onWayToCenter) {
        Center center = getCenter();
        for (int i = 0; i < 4; i += 1) {
            if (center.cells[i] != nullptr) {
                setCellDistance(center.cells[i], maxDistance);
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
        Center center = getCenter();
        for (int i = 0; i < 4; i += 1) {
            if (center.cells[i] != nullptr) {
                closest = cellMin(closest, center.cells[i]);
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
        Direction direction) {

    switch (direction) {
        case NORTH:
            y += 1;
            break;
        case EAST:
            x += 1;
            break;
        case SOUTH:
            y -= 1;
            break;
        case WEST:
            x -= 1;
            break;
    }

    bool neighboringCellExists = (
        0 <= x &&
        0 <= y &&
        x < MAZE_WIDTH &&
        y < MAZE_HEIGHT
    );

    return (neighboringCellExists ? &m_maze[x][y] : nullptr);
}

bool MackAlgoTwo::isOneCellAway(Cell* target) {

    unsigned char x = target->getX();
    unsigned char y = target->getY();
    
    if ((m_x == x) && (m_y + 1 == y) && !m_maze[m_x][m_y].isWall(NORTH)) {
        return true;
    }
    else if ((m_x == x) && (m_y - 1 == y) && !m_maze[m_x][m_y].isWall(SOUTH)) {
        return true;
    }
    else if ((m_x + 1 == x) && (m_y == y) && !m_maze[m_x][m_y].isWall(EAST)) {
        return true;
    }
    else if ((m_x - 1 == x) && (m_y == y) && !m_maze[m_x][m_y].isWall(WEST)) {
        return true;
    }
    
    return false;
}

void MackAlgoTwo::moveOneCell(Cell* target) {

    ASSERT_TR(isOneCellAway(target));

    unsigned char x = target->getX();
    unsigned char y = target->getY();
    
    Direction moveDirection = NORTH;
    if (x > m_x) {
        moveDirection = EAST;
    }
    else if (y < m_y) {
        moveDirection = SOUTH;
    }
    else if (x < m_x) {
        moveDirection = WEST;
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
        Direction direction = (m_d + i + 4) % 4;

        // If the wall is not already known
        if (!m_maze[m_x][m_y].isKnown(direction)) {

            // Read and update the wall value
            bool isWall = readWall(direction);
            setCellWall(&m_maze[m_x][m_y], direction, isWall);

            // If a neighboring cell exists, set the neighbor's wall too
            Cell* neighboringCell = getNeighboringCell(m_x, m_y, direction);
            if (neighboringCell != nullptr) {
                Direction oppositeDirection = (direction + 2) % 4;
                setCellWall(neighboringCell, oppositeDirection, isWall);
            }
        }
    }
}

bool MackAlgoTwo::readWall(Direction direction) {
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
    Center center = getCenter();
    for (int i = 0; i < 4; i += 1) {
        if (center.cells[i] != nullptr) {
            if (center.cells[i]->getX() == x && center.cells[i]->getY() == y) {
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
    m_x += (m_d == EAST  ? 1 : (m_d == WEST  ? -1 : 0));
    m_y += (m_d == NORTH ? 1 : (m_d == SOUTH ? -1 : 0));
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

void MackAlgoTwo::setColor(unsigned char x, unsigned char y, char color) {
    m_mouse->setTileColor(x, y, color);
}

void MackAlgoTwo::resetColors() {
    m_mouse->clearAllTileColor();
}

void MackAlgoTwo::colorCenter(char color) {
    Center center = getCenter();
    for (int i = 0; i < 4; i += 1) {
        if (center.cells[i] != nullptr) {
            setColor(center.cells[i]->getX(), center.cells[i]->getY(), color);
        }
    }
}

void MackAlgoTwo::setCellDistance(Cell* cell, float distance) {
    cell->info.distance = distance;
    m_mouse->setTileText(cell->getX(), cell->getY(), std::to_string(distance));
}

void MackAlgoTwo::setCellWall(Cell* cell, Direction direction, bool isWall) {
    cell->setWall(direction, isWall);
    static char directionChars[] = {'n', 'e', 's', 'w'};
    m_mouse->declareWall(cell->getX(), cell->getY(), directionChars[direction], isWall);
}

} // namespace mackAlgoTwo
