#include "MackAlgo.h"

#include <iostream>
#include <limits>

#include "CellHeap.h"
#include "Options.h"

namespace mack {

void MackAlgo::solve(sim::MouseInterface* mouse) {

    // Bookkeeping
    m_mouse = mouse;
    m_mouse->declareInterfaceType(sim::DISCRETE);

    // Initialize the mouse
    m_x = 0;
    m_y = 0;
    m_d = NORTH;
    m_onWayToCenter = true;

    // Initialize the maze
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            m_maze[x][y].setPosition(x, y);
            m_maze[x][y].setWall(NORTH, (y == MAZE_HEIGHT - 1));
            m_maze[x][y].setWall(EAST, (x == MAZE_WIDTH - 1));
            m_maze[x][y].setWall(SOUTH, (y == 0));
            m_maze[x][y].setWall(WEST, (x == 0));
        }
    }

    // Go the center, the the start, forever
    while (true) {

        // Read the walls
        readWalls(); // TODO: Only read if we haven't read before

        // Retrieve the next move
        Cell* moveTo = getNextMove();
        if (moveTo == NULL) {
            std::cout << "Unsolvable maze detected. I'm giving up..." << std::endl;
            break;
        }

        // Move the mouse by one cell
        moveOneCell(moveTo);

        // Change the destination
        if (m_onWayToCenter ? inGoal(m_x, m_y) : m_x == 0 && m_y == 0) {
            m_onWayToCenter = !m_onWayToCenter;
        }
    }
}

Cell* MackAlgo::getNextMove() {

    // Use Dijkstra's algo to determine the next best move

    // Increment the sequence number so that we know that old cell data is stale
    static int sequenceNumber = 0;
    sequenceNumber += 1;

    // Initialize the source cell
    Cell* source = &m_maze[m_x][m_y];
    source->setSequenceNumber(sequenceNumber);
    source->setSourceDirection(m_d);
    source->setParent(NULL);
    source->setDistance(0);
    initializeDestinationDistance();

    CellHeap heap;
    heap.push(source);
    while (heap.size() > 0) {

        Cell* current = heap.pop();
        current->setExamined(true);
        int x = current->getX();
        int y = current->getY();

        // We needn't explore any further
        if (current == getClosestDestinationCell()) {
            break;
        }

        // Inspect neighbors if they're not yet examined
        // NOTE: Inspecting and examining are not the same thing!
        for (int direction = 0; direction < 4; direction += 1) {
            if (!current->isWall(direction)) {
                Cell* neighbor = NULL;
                switch (direction) {
                    case NORTH:
                        neighbor = &m_maze[x][y+1];
                        break;
                    case EAST:
                        neighbor = &m_maze[x+1][y];
                        break;
                    case SOUTH:
                        neighbor = &m_maze[x][y-1];
                        break;
                    case WEST:
                        neighbor = &m_maze[x-1][y];
                        break;
                }
                if (neighbor->getSequenceNumber() != current->getSequenceNumber() || !neighbor->getExamined()) {
                    if (inspectNeighbor(current, neighbor, direction)) {
                        heap.push(neighbor);
                    }
                }
            }
        }
    }

#if (SIMULATOR)
    resetColors();
    colorCenter('G');
#endif

    Cell* prev = NULL;
    Cell* current = getClosestDestinationCell();
    while (current->getParent() != NULL) {
#if (SIMULATOR)
        setColor(current->getX(), current->getY(), 'B');
#endif
        prev = current;
        current = current->getParent();
    }
    
    return prev;
}

float MackAlgo::getTurnCost() {
    return 1.0;
}

float MackAlgo::getStraightAwayCost(int length) {
    return 1.0 / length;
}

bool MackAlgo::inspectNeighbor(Cell* current, Cell* neighbor, int direction) {

    bool pushToHeap = false;
    int x = current->getX();
    int y = current->getY();

    // Determine the cost if routed through the currect node
    float costToNeighbor = current->getDistance();
    if (current->getSourceDirection() == direction) {
        costToNeighbor += getStraightAwayCost(current->getStraightAwayLength() + 1);
    }
    else {
        costToNeighbor += getTurnCost();
    }

    // Make updates to the node
    if (neighbor->getSequenceNumber() != current->getSequenceNumber() || costToNeighbor < neighbor->getDistance()) {
        if (neighbor->getSequenceNumber() != current->getSequenceNumber()) {
            neighbor->setSequenceNumber(current->getSequenceNumber());
            pushToHeap = true;
            neighbor->setExamined(false);
        }
        neighbor->setParent(current);
        neighbor->setDistance(costToNeighbor);
        neighbor->setSourceDirection(direction);
        if (current->getSourceDirection() == direction) {
            neighbor->setStraightAwayLength(current->getStraightAwayLength() + 1);
        }
        else {
            neighbor->setStraightAwayLength(1);
        }
    }

    return pushToHeap;
}

void MackAlgo::initializeDestinationDistance() {
    float maxFloatValue = std::numeric_limits<float>::max();
    if (m_onWayToCenter) {
                m_maze[(MAZE_WIDTH - 1) / 2][(MAZE_HEIGHT - 1) / 2].setDistance(maxFloatValue);
        if (MAZE_WIDTH % 2 == 0) {
                m_maze[(MAZE_WIDTH    ) / 2][(MAZE_HEIGHT - 1) / 2].setDistance(maxFloatValue);
            if (MAZE_HEIGHT % 2 == 0) {
                m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2].setDistance(maxFloatValue);
                m_maze[ MAZE_WIDTH      / 2][ MAZE_HEIGHT      / 2].setDistance(maxFloatValue);
            }
        }
        else if (MAZE_HEIGHT % 2 == 0) {
                m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2].setDistance(maxFloatValue);
        }
    }
    else {
        m_maze[0][0].setDistance(maxFloatValue);
    }
}

Cell* MackAlgo::getClosestDestinationCell() {
    Cell* closest = NULL;
    if (m_onWayToCenter) {
                closest = min(closest, &m_maze[(MAZE_WIDTH - 1) / 2][(MAZE_HEIGHT - 1) / 2]);
        if (MAZE_WIDTH % 2 == 0) {
                closest = min(closest, &m_maze[ MAZE_WIDTH      / 2][(MAZE_HEIGHT - 1) / 2]);
            if (MAZE_HEIGHT % 2 == 0) {
                closest = min(closest, &m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2]);
                closest = min(closest, &m_maze[ MAZE_WIDTH      / 2][ MAZE_HEIGHT      / 2]);
            }
        }
        else if (MAZE_HEIGHT % 2 == 0) {
                closest = min(closest, &m_maze[(MAZE_WIDTH - 1) / 2][ MAZE_HEIGHT      / 2]);
        }
    }
    else {
        closest = &m_maze[0][0];
    }
    return closest;
}

Cell* MackAlgo::min(Cell* one, Cell* two) {
    if (one == NULL) {
        return two;
    }
    return (one->getDistance() < two->getDistance() ? one : two);
}

void MackAlgo::readWalls() {

    m_maze[m_x][m_y].setWall(m_d, m_mouse->wallFront());
    m_maze[m_x][m_y].setWall((m_d+1)%4, m_mouse->wallRight());
    m_maze[m_x][m_y].setWall((m_d+3)%4, m_mouse->wallLeft());

    if (spaceFront()) {
        getFrontCell()->setWall((m_d+2)%4, m_mouse->wallFront());
    }
    if (spaceLeft()) {
        getLeftCell()->setWall((m_d+1)%4, m_mouse->wallLeft());
    }
    if (spaceRight()) {
        getRightCell()->setWall((m_d+3)%4, m_mouse->wallRight());
    }
}

bool MackAlgo::inGoal(int x, int y) {

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

void MackAlgo::moveForward() {
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
    m_mouse->moveForward();
}

void MackAlgo::turnLeft() {
    m_d = (m_d + 3) % 4;
    m_mouse->turnLeft();
}

void MackAlgo::turnRight() {
    m_d = (m_d + 1) % 4;
    m_mouse->turnRight();
}

void MackAlgo::turnAround() {
    m_d = (m_d + 2) % 4;
    m_mouse->turnAround();
}

Cell* MackAlgo::getFrontCell() {
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

Cell* MackAlgo::getLeftCell() {
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

Cell* MackAlgo::getRightCell() {
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

Cell* MackAlgo::getRearCell() {
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

bool MackAlgo::spaceFront() {
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

bool MackAlgo::spaceLeft() {
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

bool MackAlgo::spaceRight() {
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

bool MackAlgo::isOneCellAway(Cell* target) {

    int x = target->getX();
    int y = target->getY();
    
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

void MackAlgo::moveOneCell(Cell* target) {

    if (isOneCellAway(target)) {

        int x = target->getX();
        int y = target->getY();
        
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

        // Turn the right direction
        if (moveDirection == m_d) {
            // Do nothing - we're already facing the right way
        }
        else if (moveDirection == (m_d + 1) % 4) {
            turnRight();
        }
        else if (moveDirection == (m_d + 2) % 4) {
            turnAround();
        }
        else if (moveDirection == (m_d + 3) % 4) {
            turnLeft();
        }

        // Finally, move forward one space
        moveForward();
    }
    else {
        std::cout << "Error: Tried to move to cell (" << target->getX() << ","
        << target->getY() << ") but it's not one space away from (" << m_x << ","
        << m_y << ")" << std::endl;
    }
}

void MackAlgo::setColor(int x, int y, char color) {
    m_mouse->colorTile(x, y, color);
}

void MackAlgo::resetColors() {
    m_mouse->resetColors();
}

void MackAlgo::colorCenter(char color) {
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

// TODO: Color buffer layer to get rid of the flickering

} // namespace mack
