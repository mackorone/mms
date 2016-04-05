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

// TODO: MACK - this is actually broken right now, as we don't know the cost to
// get to a tile until all the neighboring costs have been calculated

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

    // We expect a certain maze size and initial direction
    if (MAZE_WIDTH != mazeWidth || MAZE_HEIGHT != mazeHeight) {
        m_mouse->warn("Incompatible maze sizes. I'm giving up.");
        return;
    }
    if ('n' != initialDirection) {
        m_mouse->warn("Incompatible initial direction. I'm giving up.");
        return;
    }

    // Initialize the mouse
    m_x = 0;
    m_y = 0;
    m_d = NORTH;
    m_onWayToCenter = true;

    // Initialize the maze
    // TODO: MACK - init method here
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            m_maze[x][y].x = x;
            m_maze[x][y].y = y;
            m_maze[x][y].walls[NORTH] = (y == MAZE_HEIGHT - 1);
            m_maze[x][y].walls[EAST] = (x == MAZE_WIDTH - 1);
            m_maze[x][y].walls[SOUTH] = (y == 0);
            m_maze[x][y].walls[WEST] = (x == 0);
        }
    }

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
    static int sequenceNumber = 0;
    sequenceNumber += 1;

    // Initialize the source cell
    Cell* source = &m_maze[m_x][m_y];
    source->sequenceNumber = sequenceNumber;
    source->sourceDirection = m_d;
    source->parent = NULL;
    source->distance = 0;
    initializeDestinationDistance();

    CellHeap heap;
    heap.push(source);
    while (!heap.empty()) {

        // "Visit" the next closest node
        Cell* current = heap.pop();

        // TODO: MACK - make an option for this
        /*
        // We needn't explore any further
        if (current == getClosestDestinationCell()) {
            break;
        }
        */

        for (int direction = 0; direction < 4; direction += 1) {
            if (!current->walls[direction]) {
                Cell* neighbor = getCell(current->x, current->y, direction);
                checkNeighbor(current, neighbor, direction, &heap);
            }
        }
    }

    m_mouse->clearAllTileColor();
    colorCenter('G');

    // If there's no path to the destination, the maze is unsolvable
    if (getClosestDestinationCell()->parent == NULL) {
        m_mouse->warn("Unsolvable maze detected. I'm giving up...");
        return false;
    }

    // WARNING: Ugly hack to reverse the "list" of parents to a "list" of children.
    // We need this so that we can buffer the moves for the drive code.
    Cell* next = getClosestDestinationCell();
    setColor(next->x, next->y, 'B');
    Cell* current = next->parent;
    Cell* prev = current->parent;
    next->parent = NULL;
    while (prev != NULL) {
        setColor(current->x, current->y, 'B');
        current->parent = next;
        next = current;
        current = prev;
        prev = current->parent;
    }

    // Displays the color buffer
    Cell* colorCurrent = current;
    Cell* colorNext = next;
    while (colorNext != NULL && colorCurrent->known[colorNext->sourceDirection]) {
        setColor(colorNext->x, colorNext->y, 'Y');
        colorCurrent = colorNext;
        colorNext = colorNext->parent;
    }

    // First, make sure that we reset the move buffer index
    m_moveBufferIndex = 0;

    // WARNING: As a result of the ugly hack to reverse the list, we have to use
    // the parent field of the cells, though its really a child pointer at this point
    while (next != NULL && current->known[next->sourceDirection]) {
        moveOneCell(next);
        current = next;
        next = next->parent;
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

float MackAlgoTwo::getStraightAwayCost(int length) {
    return 1.0 / length;
}

void MackAlgoTwo::checkNeighbor(Cell* current, Cell* neighbor, int direction, CellHeap* heap) {

    // Determine the cost if routed through the currect node
    float costToNeighbor = current->distance;
    if (current->sourceDirection == direction) {
        costToNeighbor += getStraightAwayCost(current->straightAwayLength + 1);
    }
    else {
        costToNeighbor += getTurnCost();
    }

    // Make updates to the node
    if (neighbor->sequenceNumber != current->sequenceNumber || costToNeighbor < neighbor->distance) {
        bool pushToHeap = false;
        if (neighbor->sequenceNumber != current->sequenceNumber) {
            neighbor->sequenceNumber = current->sequenceNumber;
            pushToHeap = true;
        }
        neighbor->parent = current;
        neighbor->distance = costToNeighbor;
        neighbor->sourceDirection = direction;
        if (current->sourceDirection == direction) {
            neighbor->straightAwayLength = current->straightAwayLength + 1;
        }
            else {
                neighbor->straightAwayLength = 1;
        }
        if (pushToHeap) {
            heap->push(neighbor);
        }
        else {
            heap->update(neighbor);
        }
    }
}

// TODO: MACK - dedup these ugle methods
void MackAlgoTwo::initializeDestinationDistance() {
    float maxFloatValue = std::numeric_limits<float>::max();
    if (m_onWayToCenter) {
                m_maze[(MAZE_WIDTH - 1) / 2][(MAZE_HEIGHT - 1) / 2].distance = maxFloatValue;
        if (MAZE_WIDTH % 2 == 0) {
                m_maze[(MAZE_WIDTH    ) / 2][(MAZE_HEIGHT - 1) / 2].distance = maxFloatValue;
            if (MAZE_HEIGHT % 2 == 0) {
                m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2].distance = maxFloatValue;
                m_maze[ MAZE_WIDTH      / 2][ MAZE_HEIGHT      / 2].distance = maxFloatValue;
            }
        }
        else if (MAZE_HEIGHT % 2 == 0) {
                m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2].distance = maxFloatValue;
        }
    }
    else {
        m_maze[0][0].distance = maxFloatValue;
    }
}

Cell* MackAlgoTwo::getClosestDestinationCell() {
    Cell* closest = NULL;
    if (m_onWayToCenter) {
                closest = cellMin(closest, &m_maze[(MAZE_WIDTH - 1) / 2][(MAZE_HEIGHT - 1) / 2]);
        if (MAZE_WIDTH % 2 == 0) {
                closest = cellMin(closest, &m_maze[ MAZE_WIDTH      / 2][(MAZE_HEIGHT - 1) / 2]);
            if (MAZE_HEIGHT % 2 == 0) {
                closest = cellMin(closest, &m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2]);
                closest = cellMin(closest, &m_maze[ MAZE_WIDTH      / 2][ MAZE_HEIGHT      / 2]);
            }
        }
        else if (MAZE_HEIGHT % 2 == 0) {
                closest = cellMin(closest, &m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2]);
        }
    }
    else {
        closest = &m_maze[0][0];
    }
    return closest;
}

Cell* MackAlgoTwo::cellMin(Cell* one, Cell* two) {
    if (one == NULL) {
        return two;
    }
    return (one->distance < two->distance ? one : two);
}

void MackAlgoTwo::readWalls() {

    bool wallFront = m_mouse->wallFront();
    bool wallRight = m_mouse->wallRight();
    bool wallLeft = m_mouse->wallLeft();
/*
#else
    bool wallLeft = walls_global[0];
    bool wallFront = walls_global[1];
    bool wallRight = walls_global[2];
    movesDoneAndWallsSet = false;
#endif
*/

    if (!m_maze[m_x][m_y].known[m_d]) {
        m_maze[m_x][m_y].walls[m_d] = wallFront;
        m_maze[m_x][m_y].known[m_d] = true;
        if (spaceFront()) {
            getFrontCell()->walls[(m_d+2)%4] = wallFront;
            getFrontCell()->known[(m_d+2)%4] = true;
        }
    }

    if (!m_maze[m_x][m_y].known[(m_d+1)%4]) {
        m_maze[m_x][m_y].walls[(m_d+1)%4] = wallRight;
        m_maze[m_x][m_y].known[(m_d+1)%4] = true;
        if (spaceRight()) {
            getRightCell()->walls[(m_d+3)%4] = wallRight;
            getRightCell()->known[(m_d+3)%4] = true;
        }
    }

    if (!m_maze[m_x][m_y].known[(m_d+3)%4]) {
        m_maze[m_x][m_y].walls[(m_d+3)%4] = wallLeft;
        m_maze[m_x][m_y].known[(m_d+3)%4] = true;
        if (spaceLeft()) {
            getLeftCell()->walls[(m_d+1)%4] = wallLeft;
            getLeftCell()->known[(m_d+1)%4] = true;
        }
    }
}

bool MackAlgoTwo::inGoal(int x, int y) {

    // The goal is defined to be the center of the maze 
    // This means that it's 4 squares of length if even, 1 if odd
    
    bool horizontal = (MAZE_WIDTH - 1) / 2 == x;
    if (MAZE_WIDTH % 2 == 0) {
        horizontal = horizontal || (MAZE_WIDTH) / 2 == x;
    }

    bool vertical = (MAZE_HEIGHT - 1) / 2 == y;
    if (MAZE_HEIGHT % 2 == 0) {
        vertical = vertical || (MAZE_HEIGHT) / 2 == y;
    }

    return horizontal && vertical;
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
    switch (m_d){
        case NORTH:
            m_y += 1;
            break;
        case EAST:
            m_x += 1;
            break;
        case SOUTH:
            m_y -= 1;
            break;
        case WEST:
            m_x -= 1;
            break;
    }
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
    m_mouse->moveForward();
/*
#else
    movesBuffer[m_moveBufferIndex] = 'f';
    m_moveBufferIndex += 1;
#endif
*/
    moveForwardUpdateState();
}

void MackAlgoTwo::leftAndForward() {
    turnLeft();
    moveForward();
/*
#else
    movesBuffer[m_moveBufferIndex] = 'l';
    m_moveBufferIndex += 1;
    turnLeftUpdateState();
    moveForwardUpdateState();
#endif
*/
}

void MackAlgoTwo::rightAndForward() {
    turnRight();
    moveForward();
/*
#else
    movesBuffer[m_moveBufferIndex] = 'r';
    m_moveBufferIndex += 1;
    turnRightUpdateState();
    moveForwardUpdateState();
#endif
*/
}

void MackAlgoTwo::aroundAndForward() {
    turnAround();
    moveForward();
/*
#else
    movesBuffer[m_moveBufferIndex] = 'a';
    m_moveBufferIndex += 1;
    movesBuffer[m_moveBufferIndex] = 'f';
    m_moveBufferIndex += 1;
    turnAroundUpdateState();
    moveForwardUpdateState();
#endif
*/
}

Cell* MackAlgoTwo::getFrontCell() {
    switch (m_d) {
        case NORTH:
            return &m_maze[m_x][m_y+1];
        case EAST:
            return &m_maze[m_x+1][m_y];
        case SOUTH:
            return &m_maze[m_x][m_y-1];
        case WEST:
            return &m_maze[m_x-1][m_y];
    }
}

Cell* MackAlgoTwo::getLeftCell() {
    switch (m_d) {
        case NORTH:
            return &m_maze[m_x-1][m_y];
        case EAST:
            return &m_maze[m_x][m_y+1];
        case SOUTH:
            return &m_maze[m_x+1][m_y];
        case WEST:
            return &m_maze[m_x][m_y-1];
    }
}

Cell* MackAlgoTwo::getRightCell() {
    switch (m_d) {
        case NORTH:
            return &m_maze[m_x+1][m_y];
        case EAST:
            return &m_maze[m_x][m_y-1];
        case SOUTH:
            return &m_maze[m_x-1][m_y];
        case WEST:
            return &m_maze[m_x][m_y+1];
    }
}

Cell* MackAlgoTwo::getRearCell() {
    switch (m_d) {
        case NORTH:
            return &m_maze[m_x][m_y-1];
        case EAST:
            return &m_maze[m_x-1][m_y];
        case SOUTH:
            return &m_maze[m_x][m_y+1];
        case WEST:
            return &m_maze[m_x+1][m_y];
    }
}

Cell* MackAlgoTwo::getCell(int x, int y, int direction) {
    switch (direction) {
        case NORTH:
            return &m_maze[x][y+1];
        case EAST:
            return &m_maze[x+1][y];
        case SOUTH:
            return &m_maze[x][y-1];
        case WEST:
            return &m_maze[x-1][y];
    }
}

bool MackAlgoTwo::spaceFront() {
    switch (m_d) {
        case NORTH:
            return m_y+1 < MAZE_HEIGHT;
        case EAST:
            return m_x+1 < MAZE_WIDTH;
        case SOUTH:
            return m_y > 0;
        case WEST:
            return m_x > 0;
    }
}

bool MackAlgoTwo::spaceLeft() {
    switch (m_d) {
        case NORTH:
            return m_x > 0;
        case EAST:
            return m_y+1 < MAZE_HEIGHT;
        case SOUTH:
            return m_x+1 < MAZE_WIDTH;
        case WEST:
            return m_y > 0;
    }
}

bool MackAlgoTwo::spaceRight() {
    switch (m_d) {
        case NORTH:
            return m_x+1 < MAZE_WIDTH;
        case EAST:
            return m_y > 0;
        case SOUTH:
            return m_x > 0;
        case WEST:
            return m_y+1 < MAZE_HEIGHT;
    }
}

bool MackAlgoTwo::isOneCellAway(Cell* target) {

    int x = target->x;
    int y = target->y;
    
    if ((m_x == x) && (m_y + 1 == y) && !m_maze[m_x][m_y].walls[NORTH]) {
        return true;
    }
    else if ((m_x == x) && (m_y - 1 == y) && !m_maze[m_x][m_y].walls[SOUTH]) {
        return true;
    }
    else if ((m_x + 1 == x) && (m_y == y) && !m_maze[m_x][m_y].walls[EAST]) {
        return true;
    }
    else if ((m_x - 1 == x) && (m_y == y) && !m_maze[m_x][m_y].walls[WEST]) {
        return true;
    }
    
    return false;
}

void MackAlgoTwo::moveOneCell(Cell* target) {

    if (isOneCellAway(target)) {

        int x = target->x;
        int y = target->y;
        
        int moveDirection = NORTH;
        if (x > m_x) {
            moveDirection = EAST;
        }
        else if (y < m_y) {
            moveDirection = SOUTH;
        }
        else if (x < m_x) {
            moveDirection = WEST;
        }

        // Finally, turn and move
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
    else {
/*
#if (SIMULATOR)
        // TODO: MACK
        std::cout << "Error: Tried to move to cell (" << target->getX() << ","
        << target->getY() << ") but it's not one space away from (" << m_x << ","
        << m_y << ")" << std::endl;
#endif
*/
    }
}

void MackAlgoTwo::setColor(int x, int y, char color) {
    m_mouse->setTileColor(x, y, color);
}

void MackAlgoTwo::resetColors() {
    m_mouse->clearAllTileColor();
}

void MackAlgoTwo::colorCenter(char color) {
            setColor((MAZE_WIDTH - 1) / 2, (MAZE_HEIGHT - 1) / 2, color);
    if (MAZE_WIDTH % 2 == 0) {
            setColor( MAZE_WIDTH      / 2, (MAZE_HEIGHT - 1) / 2, color);
        if (MAZE_HEIGHT % 2 == 0) {
            setColor((MAZE_WIDTH - 1) / 2,  MAZE_HEIGHT      / 2, color);
            setColor( MAZE_WIDTH      / 2,  MAZE_HEIGHT      / 2, color);
        }
    }
    else if (MAZE_HEIGHT % 2 == 0) {
            setColor((MAZE_WIDTH - 1) / 2,  MAZE_HEIGHT      / 2, color);
    }
}

} // namespace mackAlgoTwo
