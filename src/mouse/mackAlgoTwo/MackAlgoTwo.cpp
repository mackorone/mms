#include "MackAlgoTwo.h"

#include <limits>

#include "Assert.h"
#include "History.h"
#include "Maze.h"
#include "Mode.h"

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
                setCellWall(Maze::getCell(x, y), Direction::WEST, true);
            }
            if (y == 0) {
                setCellWall(Maze::getCell(x, y), Direction::SOUTH, true);
            }
            if (x == Maze::WIDTH - 1) {
                setCellWall(Maze::getCell(x, y), Direction::EAST, true);
            }
            if (y == Maze::HEIGHT - 1) {
                setCellWall(Maze::getCell(x, y), Direction::NORTH, true);
            }
        }
    }

    // Initialize the mouse
    m_x = 0;
    m_y = 0;
    switch (initialDirection) {
        case 'n':
            m_initialDirection = Direction::NORTH;
            break;
        case 'e':
            m_initialDirection = Direction::EAST;
            break;
        default:
            m_mouse->warn("Can't start facing south or west. I'm giving up...");
            return;
    }
    m_d = m_initialDirection;
    m_mode = Mode::EXPLORE_TO_CENTER;

    // Perform a series of strategical steps ad infinitum
    while (true) {

        // Clear all tile color, and color the center
        m_mouse->clearAllTileColor();
        m_mouse->setTileColor(0, 0, 'G');
        colorCenter('G');

        // If requested, reset the mouse state and undo cell wall info
        if (resetButtonPressed()) {
            reset();
        }

        // Take pre-emptive actions, if necessary
        switch (m_mode) {
            case Mode::EXPLORE_TO_CENTER:
                if (m_x == 0 && m_y == 0) {
                    // TODO: Set mode to SOLVE_TO_CENTER potentially
                }
                break;
            case Mode::GIVE_UP:
                m_mouse->warn("Unsolvable maze detected. I'm giving up...");
                return;
        }

        // Now, depending on the mode, perform a movement
        // that will take us closer to one of four goals
        step();
    }
}

bool MackAlgoTwo::shouldColorVisitedCells() const {
    // 1 to enable, 0 to disable
    if (m_mouse->inputButtonPressed(1)) {
        if (m_mouse->inputButtonPressed(0)) {
            m_mouse->acknowledgeInputButtonPressed(1);
            m_mouse->acknowledgeInputButtonPressed(0);
            return false;
        }
        return true;
    }
    if (m_mouse->inputButtonPressed(0)) {
        m_mouse->acknowledgeInputButtonPressed(0);
    }
    return false;
}

byte MackAlgoTwo::colorVisitedCellsDelayMs() const {
    return 10;
}

bool MackAlgoTwo::resetButtonPressed() {
    return m_mouse->inputButtonPressed(2);
}

void MackAlgoTwo::acknowledgeResetButtonPressed() {
    m_mouse->acknowledgeInputButtonPressed(2);
}

twobyte MackAlgoTwo::getTurnCost() {
    switch (m_mode) {
        case Mode::EXPLORE_TO_CENTER:
        case Mode::EXPLORE_TO_ORIGIN:
            return 2;
        case Mode::SOLVE_TO_CENTER:
        case Mode::SOLVE_TO_ORIGIN:
            return 256;
        default:
            ASSERT_TR(false);
    }
}

twobyte MackAlgoTwo::getStraightAwayCost(byte length) {
    ASSERT_LT(0, length);
    ASSERT_LT(length, 16);
    switch (m_mode) {
        case Mode::EXPLORE_TO_CENTER:
        case Mode::EXPLORE_TO_ORIGIN:
            return 3;
        case Mode::SOLVE_TO_CENTER:
        case Mode::SOLVE_TO_ORIGIN:
            return 256 / length;
        default:
            ASSERT_TR(false);
    }
}

void MackAlgoTwo::reset() {

    // First, reset the position in the simulator
    m_mouse->resetPosition();

    // Then acknowledge that the button was pressed (and potentially sleep)
    acknowledgeResetButtonPressed();

    // Reset some state
    m_x = 0;
    m_y = 0;
    m_d = m_initialDirection;
    m_mode = Mode::EXPLORE_TO_CENTER;
    Maze::setStraightAwayLength(Maze::getCell(0, 0), 0);

    // Roll back some cell wall data
    while (0 < History::size()) {
        twobyte cellAndData = History::pop();
        byte cell = History::cell(cellAndData);
        byte data = History::data(cellAndData);
        for (byte direction = 0; direction < 4; direction += 1) {
            if (data >> direction + 4 & 1) {
                unsetCellWall(cell, direction, true);
            }
        }
    }
}

void MackAlgoTwo::step() {

    // If exploring, read the walls
    switch (m_mode) {
        case Mode::EXPLORE_TO_CENTER:
        case Mode::EXPLORE_TO_ORIGIN:
            readWalls();
    }

    // Get the current cell
    byte current = Maze::getCell(m_x, m_y);

    // Generate a path from the current cell to the destination
    byte start = generatePath(current);

    // Invalid path, maze not solvable
    if (start != current) {
        m_mode = Mode::GIVE_UP;
        return;
    }

    byte unknown = getFirstUnknown(start);
    if (m_mode == Mode::EXPLORE_TO_CENTER &&
        inCenter(Maze::getX(unknown), Maze::getY(unknown))) {

        // TODO: MACK - fastest path to first unknown cell in 
        m_mouse->info(
            std::string("First unknown") +
            std::string("X: ") + std::to_string(Maze::getX(unknown)) + ", " +
            std::string("Y: ") + std::to_string(Maze::getY(unknown)));

        m_mode = Mode::SOLVE_TO_ORIGIN;
        byte origin = generatePath(Maze::getCell(0, 0));
        drawPath(origin);
        m_mouse->delay(1000);
        m_mode = Mode::SOLVE_TO_CENTER;
        byte unknown2 = getFirstUnknown(origin);

        while (Maze::getCell(m_x, m_y) != unknown2) {
            readWalls();
        }
    }
    if (m_mode == Mode::EXPLORE_TO_ORIGIN &&
        inOrigin(Maze::getX(unknown), Maze::getY(unknown))) {
        
        m_mouse->info(
            std::string("First unknown") +
            std::string("X: ") + std::to_string(Maze::getX(unknown)) + ", " +
            std::string("Y: ") + std::to_string(Maze::getY(unknown)));

    }

    // Draw the path from the current position to the destination
    drawPath(start);

    // Move along the path as far as possible
    followPath(start);

    // Update the mode if we've reached the destination
    if (m_mode == Mode::EXPLORE_TO_CENTER && inCenter(m_x, m_y)) {
        m_mode = Mode::EXPLORE_TO_ORIGIN;
    }
    if (m_mode == Mode::EXPLORE_TO_ORIGIN && inOrigin(m_x, m_y)) {
        m_mode = Mode::EXPLORE_TO_CENTER;
    }
}

byte MackAlgoTwo::generatePath(byte start) {

    // Reset the sequence bit of all cells
    for (byte x = 0; x < Maze::WIDTH; x += 1) {
        for (byte y = 0; y < Maze::HEIGHT; y += 1) {
            Maze::setDiscovered(Maze::getCell(x, y), false);
        }
    }

    // Initialize the starting cell
    Maze::setDiscovered(start, true);
    setCellDistance(start, 0);

    // This is nuanced - when we are determining whether or not a movement
    // continues the straightaway path, we inspect the previous cells "next"
    // pointer, which really points *that* cell's previous cell. In the case of
    // the starting cell, we assume that the previous cell is directly behind
    // us, so that the straightaway distance is properly calculated.
    Maze::setNextDirection(start, getOppositeDirection(m_d));
    Maze::clearNext(start);

    // Reset the destination cell distances
    resetDestinationCellDistances();

    // Dijkstra's algo
    ASSERT_EQ(Heap::size(), 0);
    Heap::push(start);
    while (0 < Heap::size()) {
        byte cell = Heap::pop();
        for (byte direction = 0; direction < 4; direction += 1) {
            if (!Maze::isWall(cell, direction)) {
                checkNeighbor(cell, direction);
            }
        }
        if (shouldColorVisitedCells()) {
            m_mouse->delay(colorVisitedCellsDelayMs());
            m_mouse->setTileColor(Maze::getX(cell), Maze::getY(cell), 'Y');
        }
        if (cell == getClosestDestinationCell()) {
            Heap::clear();
            break;
        }
    }

    // TODO: MACK
    /*
    // If there's no path to the destination, the maze is unsolvable
    if (!Maze::hasNext(getClosestDestinationCell())) {
        m_mode = Mode::GIVE_UP;
    }
    */

    // Reverse the linked list from the destination to the start (which we
    // built during our execution of Dijkstra's algo) into a linked list from
    // the start to the destination (which we use to instruct the robot's
    // movements).
    return reverseLinkedList(getClosestDestinationCell());
}

void MackAlgoTwo::drawPath(byte start) {

    // This is probably a little two cutesy for it's own good. Oh well...
    byte current = start;
    for (byte i = 0; i < 2; i += 1) {
        while (Maze::hasNext(current)) {
            byte next = getNeighboringCell(current, Maze::getNextDirection(current));
            // Draw the "known" moves
            if (i == 0) {
                if (!Maze::isKnown(current, Maze::getNextDirection(current))) {
                    break;
                }
                m_mouse->setTileColor(Maze::getX(next), Maze::getY(next), 'V');
            }
            // Draw the "intended" moves
            else {
                m_mouse->setTileColor(Maze::getX(next), Maze::getY(next), 'B');
            }
            current = next;
        }
    }
}

void MackAlgoTwo::followPath(byte start) {

    // Move forward as long as we know we won't collide with a wall
    byte current = start;
    while (Maze::hasNext(current) && Maze::isKnown(current, Maze::getNextDirection(current))) {

        // Move to the next cell and advance our pointers
        byte next = getNeighboringCell(current, Maze::getNextDirection(current));
        moveOneCell(next);
        current = next;

        // Inform the History class that the mouse has moved a cell
        History::move();

        // If the reset button was pressed, we should stop moving
        if (resetButtonPressed()) {
            break;
        }
    }
}

byte MackAlgoTwo::getFirstUnknown(byte start) {
    byte current = start;
    while (Maze::hasNext(current) &&
           Maze::isKnown(current, Maze::getNextDirection(current))) {
        current = getNeighboringCell(current, Maze::getNextDirection(current));
    }
    return current;
}

void MackAlgoTwo::checkNeighbor(byte cell, byte direction) {

    // Retrieve the neighboring cell, and the direction that would take us from
    // the neighboring cell to the current cell (which is the opposite of the
    // direction that takes us from the current cell to the neighboring cell)
    byte neighbor = getNeighboringCell(cell, direction);
    byte directionFromNeighbor = getOppositeDirection(direction);

    // Determine the cost if routed through the current cell
    twobyte costToNeighbor = Maze::getDistance(cell) + (
        Maze::getNextDirection(cell) == directionFromNeighbor ?
        getStraightAwayCost(Maze::getStraightAwayLength(cell) + 1) :
        getTurnCost()
    );

    // Make updates to the neighbor cell if necessary
    if (!Maze::getDiscovered(neighbor) ||
        costToNeighbor < Maze::getDistance(neighbor)) {

        // Update the distance, next direction, and straight away length
        setCellDistance(neighbor, costToNeighbor);
        Maze::setNextDirection(neighbor, directionFromNeighbor);
        Maze::setStraightAwayLength(neighbor, (
            Maze::getNextDirection(cell) == directionFromNeighbor ?
            Maze::getStraightAwayLength(cell) + 1 : 1
        ));

        // Either discover (and push) the cell, or just update it
        if (!Maze::getDiscovered(neighbor)) {
            Maze::setDiscovered(neighbor, true);
            Heap::push(neighbor);
        }
        else {
            Heap::update(neighbor);
        }
    }
}

byte MackAlgoTwo::reverseLinkedList(byte cell) {
    byte closest = cell;
    byte direction = Maze::getNextDirection(closest);
    byte current = getNeighboringCell(closest, direction);
    Maze::clearNext(closest);
    while (Maze::hasNext(current)) {
        byte temp = Maze::getNextDirection(current);
        Maze::setNextDirection(current, getOppositeDirection(direction));
        direction = temp;
        closest = current;
        current = getNeighboringCell(current, direction);
    }
    Maze::setNextDirection(current, getOppositeDirection(direction));
    return current;
}

bool MackAlgoTwo::inCenter(byte x, byte y) {
    for (byte xx = Maze::CLLX; xx <= Maze::CURX; xx += 1) {
        for (byte yy = Maze::CLLY; yy <= Maze::CURY; yy += 1) {
            if (x == xx && y == yy) {
                return true;
            }
        }
    }
    return false;
}

bool MackAlgoTwo::inOrigin(byte x, byte y) {
    return x == 0 && y == 0;
}

void MackAlgoTwo::colorCenter(char color) {
    for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
        for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
            m_mouse->setTileColor(x, y, color);
        }
    }
}

void MackAlgoTwo::resetDestinationCellDistances() {
    static twobyte maxDistance = std::numeric_limits<twobyte>::max();
    if (m_mode == Mode::EXPLORE_TO_CENTER || m_mode == Mode::SOLVE_TO_CENTER) {
        for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
            for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
                setCellDistance(Maze::getCell(x, y), maxDistance);
            }
        }
    }
    else {
        setCellDistance(Maze::getCell(0, 0), maxDistance);
    }
}

byte MackAlgoTwo::getClosestDestinationCell() {
    byte closest = Maze::getCell(Maze::CLLX, Maze::CLLY);
    if (m_mode == Mode::EXPLORE_TO_CENTER || m_mode == Mode::SOLVE_TO_CENTER) {
        for (byte x = Maze::CLLX; x <= Maze::CURX; x += 1) {
            for (byte y = Maze::CLLY; y <= Maze::CURY; y += 1) {
                byte other = Maze::getCell(x, y);
                if (Maze::getDistance(other) < Maze::getDistance(closest)) {
                    closest = other;
                }
            }
        }
    }
    else {
        closest = Maze::getCell(0, 0);
    }
    return closest;
}

byte MackAlgoTwo::getOppositeDirection(byte direction) {
    switch (direction) {
        case Direction::NORTH:
            return Direction::SOUTH;
        case Direction::EAST:
            return Direction::WEST;
        case Direction::SOUTH:
            return Direction::NORTH;
        case Direction::WEST:
            return Direction::EAST;
        default:
            ASSERT_TR(false);
    }
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
            return Maze::getCell(x, y + 1);
        case Direction::EAST:
            return Maze::getCell(x + 1, y);
        case Direction::SOUTH:
            return Maze::getCell(x, y - 1);
        case Direction::WEST:
            return Maze::getCell(x - 1, y);
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

    // Record the cell and wall data for the History
    byte cell = Maze::getCell(m_x, m_y);
    byte data = 0;

    // For each of [left, front, right]
    for (int i = -1; i <= 1; i += 1) {
        byte direction = (m_d + i + 4) % 4;

        // If the wall is not already known
        if (!Maze::isKnown(m_x, m_y, direction)) {

            // Read and update the wall value
            bool isWall = readWall(direction);
            setCellWall(Maze::getCell(m_x, m_y), direction, isWall);

            // Set the "learned" bit, as well as "walls" bit
            data |= 1 << direction + 4;
            if (isWall) {
                data |= 1 << direction;
            }
        }
    }

    // Actually add the learned cell walls to the History
    History::add(cell, data);
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

void MackAlgoTwo::setCellDistance(byte cell, twobyte distance) {
    Maze::setDistance(cell, distance);
    m_mouse->setTileText(Maze::getX(cell), Maze::getY(cell), std::to_string(distance));
}

void MackAlgoTwo::setCellWall(byte cell, byte direction, bool isWall, bool bothSides) {
    Maze::setWall(cell, direction, isWall);
    static char directionChars[] = {'n', 'e', 's', 'w'};
    m_mouse->declareWall(Maze::getX(cell), Maze::getY(cell), directionChars[direction], isWall);
    if (bothSides && hasNeighboringCell(cell, direction)) {
        byte neighboringCell = getNeighboringCell(cell, direction);
        setCellWall(neighboringCell, getOppositeDirection(direction), isWall, false);
    }
}

void MackAlgoTwo::unsetCellWall(byte cell, byte direction, bool bothSides) {
    Maze::unsetWall(cell, direction);
    static char directionChars[] = {'n', 'e', 's', 'w'};
    m_mouse->undeclareWall(Maze::getX(cell), Maze::getY(cell), directionChars[direction]);
    if (bothSides && hasNeighboringCell(cell, direction)) {
        byte neighboringCell = getNeighboringCell(cell, direction);
        unsetCellWall(neighboringCell, getOppositeDirection(direction), false);
    }
}

} // namespace mackAlgoTwo
