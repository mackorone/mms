#include <stdio.h>
#include <stdlib.h>

#include "Cell.h"
#include "Cellmod.h"
#include "CellStack.h"
#include "FloodFill.h"
#include "History.h"
#include "List.h"
#include "SimpleCellmod.h"

int main() { solve(); } // TODO: Get rid of this

void solve() {

    firstHInitialize = true;

    // Initialize the x and y positions of the cells
    for (int x = 0; x < MAZE_SIZE_X; x++) {
        for (int y = 0; y < MAZE_SIZE_Y; y++) {
            m_cells[x][y].m_x = x;
            m_cells[x][y].m_y = y;
        }
    }

    if (ALGO_COMPARE) {
        simpleSolve();
        extensiveSolve();
        exit(0);
    }

    // Otherwise, pick one
    // justFloodFill();
    // simpleSolve();
    extensiveSolve();

}

void justFloodFill() {

    initialize();

    while (!inGoal(m_x,m_y)) {
        walls();
        flood(m_x, m_y);
        moveTowardsGoal();
    }

}

void simpleSolve() {

    while (true) {

        // Initialize the cell fields
        initialize();

        // Do the basic floodfill and store the path
        struct CellStack *path = createStack();
        bffExplore(path);

        // If the explore was not successful (i.e., reset request was made) then start over
        if (!m_explored) {
            continue;
        }

        int s_steps = m_steps;
        if (ALGO_COMPARE) {
            printf("Simple explore in %d steps\n",m_steps);
        }

        // Solve the maze as quickly as possible.
        bffVictory(path);

        destroyStack(path);

        if (ALGO_COMPARE) {
            printf("Simple solve in %d steps\n", (m_steps - s_steps)/2);
            return;
        }

    }

}

void extensiveSolve() {

    // Loop to allow for reset requests to be honored
    while (true) {

        // Initialize all mutable fields, including the Cells' walls
        // and distances as well as the mouse-related fields
        initialize();

        // Augmented DFS - Explore the maze to its entirety, returning to the start
        explore();

        // If the explore was not successful (i.e., reset request was made) then start over
        if (!m_explored) {
            continue;
        }

        int s_steps = m_steps;
        if (ALGO_COMPARE) {
            printf("Extensive explore in %d steps\n", m_steps);
        }

        // Once we know everything about the maze, solve the maze as quickly as possible.
        victory();

        if (ALGO_COMPARE) {
            printf("Extensive solve in %d steps\n", (m_steps - s_steps)/2);
            return;
        }
    }

}

void printDistances() {
    printf("\n");
    for (int y = MAZE_SIZE_Y-1; y >= 0; y--) {
        for (int x = 0; x < MAZE_SIZE_X; x++) {
            if (m_cells[x][y].m_distance < 100) {
                if (m_cells[x][y].m_distance < 10) {
                    printf(" ");
                }
                printf(" ");
            }
            printf("%d ",m_cells[x][y].m_distance);
        }
        printf("\n");
    }
}

void printWalls() {
    printf("\n");
    for (int y = MAZE_SIZE_Y-1; y >= 0; y--) {
        for (int x = 0; x < MAZE_SIZE_X; x++) {
            int wallSum = 0;
            wallSum += 1*m_cells[x][y].m_walls[NORTH];
            wallSum += 2*m_cells[x][y].m_walls[EAST];
            wallSum += 4*m_cells[x][y].m_walls[SOUTH];
            wallSum += 8*m_cells[x][y].m_walls[WEST];
            if (wallSum < 10) {
                printf(" ");
            }
            printf("%d ",wallSum);
        }
        printf("\n");
    }
}

void initialize() {

    // NOTE: Doesn't work for odd sized mazes.

    // Initialize the distance values for the maze
    for (int y = 0; y < MAZE_SIZE_Y/2; y++) {
        int distance = MAZE_SIZE_X/2 + MAZE_SIZE_Y/2 - 2 - y;
        for(int x = 0; x < MAZE_SIZE_X/2; x++) {
            m_cells[x][y].m_distance = distance; // Bottom-left
            m_cells[x][MAZE_SIZE_Y-1-y].m_distance = distance; // Upper-left
            m_cells[MAZE_SIZE_X-1-x][MAZE_SIZE_Y-1-y].m_distance = distance; // Upper-right
            m_cells[MAZE_SIZE_X-1-x][y].m_distance = distance; // Bottom-right
            distance -= 1;
        }
    }

    // Initialize other values for the Cells
    for (int y = 0; y < MAZE_SIZE_Y; y++) {
        for (int x = 0; x < MAZE_SIZE_X; x++) {

            // Set the walls values
            m_cells[x][y].m_walls[SOUTH] = (y == 0);
            m_cells[x][y].m_walls[NORTH] = (y == MAZE_SIZE_Y-1);
            m_cells[x][y].m_walls[WEST] = (x == 0);
            m_cells[x][y].m_walls[EAST] = (x == MAZE_SIZE_X-1);

            // Set the inspected values of the walls
            m_cells[x][y].m_wallsInspected[SOUTH] = (y == 0);
            m_cells[x][y].m_wallsInspected[NORTH] = (y == MAZE_SIZE_Y-1);
            m_cells[x][y].m_wallsInspected[WEST] = (x == 0);
            m_cells[x][y].m_wallsInspected[EAST] = (x == MAZE_SIZE_X-1);

            // Set the prev and explored values of the Cells
            m_cells[x][y].m_prev = NULL;
            m_cells[x][y].m_explored = false;
            m_cells[x][y].m_traversed = false;

        }
    }

    // Initialize mouse-related fields
    m_x = 0; // Initialize the x position of the mouse
    m_y = 0; // Initialize the y position of the mouse
    m_d = 0; // Initialize the direction position of the mouse
    m_steps = 0; // Initialize the mouse steps
    m_explored = false; // Initialize the exploredness of the maze
    m_centerReached = false; // At the start, we haven't made it to the center
    m_checkpointReached = true; // We begin at the origin, the first checkpoint
    h_initialize(&m_history, SHORT_TERM_MEM, &m_cells[0][0], firstHInitialize); // Initialize the History object
    firstHInitialize = false;

}

void victory() {

    // Loop forever, continue going to the beginning and solving
    while (true) {

      struct CellStack *optimalPath = createStack();

        // Solve the maze (while request hasn't been made)
        while (!inGoal(m_x, m_y) && !(_undoRequested() || _resetRequested())) {
            push(optimalPath,&m_cells[m_x][m_y]);
            moveTowardsGoal();
        }

        // Return to start (while request hasn't been made)
        while ((m_x != 0 || m_y != 0) && !(_undoRequested() || _resetRequested())) {
            struct Cell *prev = top(optimalPath);
            pop(optimalPath);
            moveOneCell(prev);
        }

        destroyStack(optimalPath);

        if (!(_undoRequested() || _resetRequested())) {
            if (m_d == 2) {
                turnAround();
            } else { // m_d == 3
                turnRight();
            }
        }

        // If a reset request is made, return so we can re-init and re-explore
        if (checkRequestVictory()) {
            return;
        }

        if (ALGO_COMPARE) {
            return;
        }
    }

}

void walls() {

    // Sets the wall values for the current cell
    m_cells[m_x][m_y].m_walls[m_d] = _wallFront();
    m_cells[m_x][m_y].m_walls[(m_d+1)%4] = _wallRight();
    m_cells[m_x][m_y].m_walls[(m_d+3)%4] = _wallLeft();

    // Sets the wallInspected values for the current cell
    m_cells[m_x][m_y].m_wallsInspected[m_d] = true;
    m_cells[m_x][m_y].m_wallsInspected[(m_d+1)%4] = true;
    m_cells[m_x][m_y].m_wallsInspected[(m_d+3)%4] = true;

    // Sets the wall and wallInspected values for the surrounding cells

    struct Cell *temp;

    if (spaceFront()) {
        temp = getFrontCell();
        temp->m_walls[(m_d+2)%4] = _wallFront();
        temp->m_wallsInspected[(m_d+2)%4] = true;
        checkDeadEnd(temp);
    }

    if (spaceLeft()) {
        temp = getLeftCell();
        temp->m_walls[(m_d+1)%4] = _wallLeft();
        temp->m_wallsInspected[(m_d+1)%4] = true;
        checkDeadEnd(temp);
    }

    if (spaceRight()) {
        temp = getRightCell();
        temp->m_walls[(m_d+3)%4] = _wallRight();
        temp->m_wallsInspected[(m_d+3)%4] = true;
        checkDeadEnd(temp);
    }

}

void checkDeadEnd(struct Cell *cell) {

    // First we check to see if we've discovered three walls for the particular
    // Cell. We don't have to check the wallInspected values of each of the walls
    // since walls themselves are not set to true until they are discovered - thus
    // we can deduce that any walls we've found have also been inspected. The reason
    // that we check for the presence of three walls is that only dead-end Cells have
    // three walls. Thus, we can treat any Cells with threee walls as if it actually
    // has four walls. In doing so, we cause a "chain reaction" of sorts, allowing
    // neighboring Cells to be classified as dead-ends as well.

    int walls = 0;
    for (int i = 0; i < 4; i++) {
        if (cell->m_walls[i]) {
            walls++;
        }
    }

    if (walls == 3) {
        for (int i = 0; i < 4; i++) {
            if (!cell->m_wallsInspected[i]) {

                // Again, if we know a Cell has three walls, it's a dead-end and can be
                // treated as if it has four walls. Thus, although a Cell might not have
                // a fourth wall, we allow neighboring Cells (or Cells on a path to the
                // dead-end Cell) to be treated as dead-end Cells as well.

                cell->m_walls[i] = true;
                cell->m_wallsInspected[i] = true;

                // Update the neighboring cells
                switch(i) {
                    case 0:
                        m_cells[cell->m_x][cell->m_y + 1].m_walls[2] = true;
                        m_cells[cell->m_x][cell->m_y + 1].m_wallsInspected[2] = true;
                        checkDeadEnd(&m_cells[cell->m_x][cell->m_y + 1]);
                        break;
                    case 1:
                        m_cells[cell->m_x + 1][cell->m_y].m_walls[3] = true;
                        m_cells[cell->m_x + 1][cell->m_y].m_wallsInspected[3] = true;
                        checkDeadEnd(&m_cells[cell->m_x + 1][cell->m_y]);
                        break;
                    case 2:
                        m_cells[cell->m_x][cell->m_y - 1].m_walls[0] = true;
                        m_cells[cell->m_x][cell->m_y - 1].m_wallsInspected[0] = true;
                        checkDeadEnd(&m_cells[cell->m_x][cell->m_y - 1]);
                        break;
                    case 3:
                        m_cells[cell->m_x - 1][cell->m_y].m_walls[1] = true;
                        m_cells[cell->m_x - 1][cell->m_y].m_wallsInspected[1] = true;
                        checkDeadEnd(&m_cells[cell->m_x - 1][cell->m_y]);
                        break;
                }
                break;
            }
        }
    }
}

void flood(int x, int y) {
    
    // *DO NOT* flood the cells in the goal region of the maze- there is no
    // information that we can gain about the distance values in the goal,
    // and we can only corrupt the current distance values anyways (they
    // can't and shouldn't change from 0)
    if (!inGoal(x, y)) {

        // Initialize distance values for surrounding cells
        int northDistance = MAZE_SIZE_X*MAZE_SIZE_Y;
        int eastDistance = MAZE_SIZE_X*MAZE_SIZE_Y;
        int southDistance = MAZE_SIZE_X*MAZE_SIZE_Y;
        int westDistance = MAZE_SIZE_X*MAZE_SIZE_Y;

        // Obtain actual values if possible
        if (!m_cells[x][y].m_walls[NORTH]) {
            northDistance = m_cells[x][y+1].m_distance;
        }
        if (!m_cells[x][y].m_walls[EAST]) {
            eastDistance = m_cells[x+1][y].m_distance;
        }
        if (!m_cells[x][y].m_walls[SOUTH]) {
            southDistance = m_cells[x][y-1].m_distance;
        }
        if (!m_cells[x][y].m_walls[WEST]) {
            westDistance = m_cells[x-1][y].m_distance;
        }

        // Check to see if the distance value is the min plus one
        if (m_cells[x][y].m_distance != min(northDistance, eastDistance, southDistance, westDistance) + 1) {

            // Set the value to the min plus one
            m_cells[x][y].m_distance = min(northDistance, eastDistance, southDistance, westDistance) + 1;

            if (!m_cells[x][y].m_walls[NORTH]) {
                flood(x, y + 1);
            }
            if (!m_cells[x][y].m_walls[EAST]) {
                flood(x + 1, y);
            }
            if (!m_cells[x][y].m_walls[SOUTH]) {
                flood(x, y - 1);
            }
            if (!m_cells[x][y].m_walls[WEST]) {
                flood(x - 1, y);
            }
        }
    }
}

void moveTowardsGoal() {
    
    // Invariant: One of the adjacent cells is guarenteed to have a lower value, so
    //            it's safe to initialize the minimum distance to current distance
    int minDistance = m_cells[m_x][m_y].m_distance; // Initialize distance
    int relDirection = 0; // Initialize direction

    if (!m_cells[m_x][m_y].m_walls[(m_d+3)%4]) { // Check left
        if (getLeftCell()->m_distance <= minDistance) {
            minDistance = getLeftCell()->m_distance;
            relDirection = 3;
        }
    }
    if (!m_cells[m_x][m_y].m_walls[(m_d+2)%4]) { // Check behind
        if (getRearCell()->m_distance <= minDistance) {
            minDistance = getRearCell()->m_distance;
            relDirection = 2;
        }
    }
    if (!m_cells[m_x][m_y].m_walls[(m_d+1)%4]) { // Check right
        if (getRightCell()->m_distance <= minDistance) {
            minDistance = getRightCell()->m_distance;
            relDirection = 1;
        }
    }
    if (!m_cells[m_x][m_y].m_walls[m_d]) { // Check front
        if (getFrontCell()->m_distance <= minDistance) {
            minDistance = getFrontCell()->m_distance;
            relDirection = 0;
        }
    }
    
    switch (relDirection) {
        case 1: // turn right
            turnRight();
            break;
        case 2: // turn around
            turnAround();
            break;
        case 3: // turn left
            turnLeft();
            break;
    }

    moveForward(); // Move the robot forward

}

void moveForward() {

    // NOTE: There is no preliminary check for walls or for limits in this
    //       function - this is the responsibility of the caller
    
    // First, update our internal representation of location
    switch (m_d) {
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

    _wallFront(); // Courtesy of Tomasz
    _wallLeft();
    _wallRight();

    // Actually move the mouse forward in the simulation
    _moveForward();

    // Increment the number of steps
    m_steps++;

    // If we're still exploring append to explore path. Although it won't hurt
    // anything to append to the history after we're done exploring, but it
    // doesn't help and would just be wasting time. We include the condition
    // that the checkpoint must be reached so that we're not appending to
    // the path unnecessarily.
    if (!m_explored && m_checkpointReached) {
        moved(&m_history);
    }

}

void turnRight() {
    m_d = (m_d + 1) % 4; // Update internal representation
    _turnRight(); // Move the mouse
    m_steps++; // Lastly, increment the number of steps
}

void turnLeft() {
    m_d = (m_d + 3) % 4; // Update internal representation
    _turnLeft(); // Move the mouse
    m_steps++; // Lastly, increment the number of steps
}

void turnAround() {
    m_d = (m_d + 2) % 4; // Update internal representation
    _turnAround(); // Move the mouse
    m_steps += 2; // Lastly, increment the number of steps
}

bool inGoal(int x, int y) {

    // The goal is defined to be the center of the maze 
    // This means that it's 4 squares of length if even, 1 if odd
    
    bool horizontal = (MAZE_SIZE_X - 1) / 2 == x;
    if (MAZE_SIZE_X % 2 == 0) {
        horizontal = horizontal || (MAZE_SIZE_X) / 2 == x;
    }

    bool vertical = (MAZE_SIZE_Y - 1) / 2 == y;
    if (MAZE_SIZE_Y % 2 == 0) {
        vertical = vertical || (MAZE_SIZE_Y) / 2 == y;
    }

    return horizontal && vertical;

}

int min(int one, int two, int three, int four) {
    int firstMin = one;
    int secondMin = three;
    if (two <= one) {
        firstMin = two;
    }
    if (four <= three) {
        secondMin = four;
    }
    if (firstMin <= secondMin) {
        return firstMin;
    }
    else{
        return secondMin;
    }
}

struct Cell * getFrontCell() {
    switch (m_d) {
        case NORTH:
            return &m_cells[m_x][m_y+1];
        case EAST:
            return &m_cells[m_x+1][m_y];
        case SOUTH:
            return &m_cells[m_x][m_y-1];
        case WEST:
            return &m_cells[m_x-1][m_y];
    }
}

struct Cell * getLeftCell() {
    switch (m_d) {
        case NORTH:
            return &m_cells[m_x-1][m_y];
        case EAST:
            return &m_cells[m_x][m_y+1];
        case SOUTH:
            return &m_cells[m_x+1][m_y];
        case WEST:
            return &m_cells[m_x][m_y-1];
    }
}

struct Cell * getRightCell() {
    switch (m_d) {
        case NORTH:
            return &m_cells[m_x+1][m_y];
        case EAST:
            return &m_cells[m_x][m_y-1];
        case SOUTH:
            return &m_cells[m_x-1][m_y];
        case WEST:
            return &m_cells[m_x][m_y+1];
    }
}

struct Cell * getRearCell() {
    switch (m_d) {
        case NORTH:
            return &m_cells[m_x][m_y-1];
        case EAST:
            return &m_cells[m_x-1][m_y];
        case SOUTH:
            return &m_cells[m_x][m_y+1];
        case WEST:
            return &m_cells[m_x+1][m_y];
    }
}

bool spaceFront() {
    switch (m_d) {
        case NORTH:
            return m_y+1 < MAZE_SIZE_Y;
        case EAST:
            return m_x+1 < MAZE_SIZE_X;
        case SOUTH:
            return m_y > 0;
        case WEST:
            return m_x > 0;
    }
}

bool spaceLeft() {
    switch (m_d) {
        case NORTH:
            return m_x > 0;
        case EAST:
            return m_y+1 < MAZE_SIZE_Y;
        case SOUTH:
            return m_x+1 < MAZE_SIZE_X;
        case WEST:
            return m_y > 0;
    }
}

bool spaceRight() {
    switch (m_d) {
        case NORTH:
            return m_x+1 < MAZE_SIZE_X;
        case EAST:
            return m_y > 0;
        case SOUTH:
            return m_x > 0;
        case WEST:
            return m_y+1 < MAZE_SIZE_Y;
    }
}

void explore() {

   /*
    *  The foundation of the explore method is a simple DFS search. However,
    *  DFS is not terribly efficient in this context and can be improved upon
    *  greatly. See below for improvements, failures and ideas. Any testing or
    *  comparison can be done with the exploreBeta method.
    *
    *  IMPROVEMENTS:
    *  1.) When tracing back to the next parent node with neighbors, check to
    *      see if, at any point in the retracing, that you're next to the
    *      target node. If so, there is no need to retrace all the way back to
    *      the parent node. Rather, we can simply proceed to the target node
    *      from where we currently are.
    *  2.) Keep track of which walls have been inspected. If all walls of a Cell
    *      have been inspected, there is no need to explicitly move the mouse
    *      into that particular Cell. Thus, after popping a Cell off of the stack,
    *      check to see if all of its walls have been explored. If so, pop the
    *      next Cell off of the stack and proceed. 
    *  3.) Check neighbors of untraversed but fully explored Cells for the
    *      possibility of short-circuiting the retracing opertation.
    *
    *  FAILURES:
    *  1.) Updating the prev values as cells were rediscovered. This performed
    *      worse in every test case
    *  2.) Choosing to explore the neighbor with the lowest cell value, as
    *      opposed to a R-F-L priority (or L-F-R priority)
    *  3.) Choosing to explore the neighbor with the highest cell value, as
    *      opposed to a R-F-L priority (or L-F-R priority)
    *  4.) Choosing to explore the neighbor with a different directional
    *      priority, other than L-F-R or R-F-L
    *
    *  IDEAS:
    *  1.) Do we need to explore cells that are far away from the goal?
    *
    *
    *  The execution of the explore method works as follows:
    *
    *  while (not fully explored):
    *      1) Check for undo and reset request
    *      2) Find next valid target
    *      3) Retrace to target
    *      4) Perform updates
    */

    // Push unexplored nodes onto a stack
    struct CellStack *unexplored = createStack();

    // The initial square is at (0, 0)
    push(unexplored, &m_cells[0][0]);

    // The explore method needs to explore all Cells, so we loop until unexplored is empty.
    // The other condition ensures that any requests, regardless of whether or not the
    // stack was empty at the time of the request (which could happen as we visit the
    // the last target, or in other weird cases), are honored and handled appropriately.
    while (!isEmpty(unexplored) || (_undoRequested() || _resetRequested()) ) {

        if (_resetRequested()) {
            destroyStack(unexplored);
            _resetPosition();
            _resetHonored();
            return;
        }

        // As it stands right now, if the mouse screws up going into the final few cells of
        // the maze and we don't hit reset before it thinks it solved the maze, we don't really
        // have a good way to undo the changes.
    
        if (_undoRequested()) {

            // We check to see if we've moved at least SHORT_TERM_MEM number of times. If so, then
            // we can go back that many steps to the state that existed right BEFORE those steps

            m_x = 0;
            m_y = 0;
            m_d = 0;
            _resetPosition();

            // We haven't yet gotten to the checkpoint. If another undo request is made,
            // don't reset the checkpoint. Simply try to get back to the same checkpoint.
            m_checkpointReached = false;

            // We must call undoHonored() directly after we decide to honor the request
            // so that we may detect more undo requests during our attempt to continue
            // to the checkpoint the maze.
            _undoHonored();

            // If an undo is requested within the first few steps, we can simply
            // re-explore the maze since we haven't made an real progress yet.
            // Doing things this way ensures that some corner cases are handled.
            destroyStack(unexplored);
            if (getCheckpointCell(&m_history) == &m_cells[0][0]) {
                return;
            }

            // Retrieve the old stack image.
            unexplored = getCheckpointStack(&m_history);

            // Retrieve the path to get back to the current cell.
            struct CellStack *path = getCheckpointPath(&m_history);

            // Clear the modified cells.
            resetModifiedCells(&m_history);

            // Moves to the checkpoint and checks for requests along the way.
            if (proceedToCheckpoint(path)) {
                destroyStack(path);
                if (_resetRequested()) {
                    _resetPosition();
                    _resetHonored();
                    return;
                }
                else {
                    // If another undo is requested while this undo is still being completed,
                    // then we should simply redo our attempts to get to the checkpoint cell
                    continue;
                }
            }
            
            destroyStack(path);

            // As this point, we've reached the checkpoint and can procees to the next target
            m_checkpointReached = true;
            continue;
        }

        // Begin by assuming that all walls of the Cell that we intend to explore
        // have already been inspected. Thus, we assume that we have no need to
        // actually explore that Cell. We check the wallInspected values of each
        // of the walls of that Cell to confirm (or refute) our assumption. Once
        // we find a Cell for which not all of the walls have been inspected, we
        // proceed as normal.

        bool allWallsInspected = true;
        struct Cell *target = NULL;

        // The first condition for looping, namely that all not all walls be
        // inspected, is vital to the functioning of the rest of the explore
        // algorithm. Once this loop exits, we can be assured that either we've
        // explored everything, or that the target has not yet been traversed
        // (but will be traversed).

        while (allWallsInspected && !isEmpty(unexplored)) {

            // First, pop the target cell off of the stack
            target = top(unexplored); 
            pop(unexplored);
    
            // Next, check to see if all of the walls have been inspected.
            // If so, no need to explore that cell - get a new target
            for (int i = 0; i < 4; i++) {
                allWallsInspected = allWallsInspected && target->m_wallsInspected[i];
            } 
            
            // Although we don't need to explicitly explore a Cell if we've
            // already inspected all of its wall values, we still need to
            // flood it (and its neighbors) with a proper distance value.
            // In addition, we need to mark it as explored, acknowledging
            // that once we've inspected all walls, we've essentially explored
            // that Cell. If we don't mark the Cell as explored, then we may
            // accidentally assign it a maximum distance value, since we assign
            // these values based on whether or not a Cell was explored or not.
            // Note that there is no danger of setting an unreachable Cell as
            // explored, since an unreachable cell will be pushed onto the stack
            // in the first place.

            if (allWallsInspected) {
                target->m_explored = true;
                flood(target->m_x, target->m_y);
            }
        }

        // We check for !allWallsInspected here in case the last thing on the stack had
        // all of its walls inspected. In that case, we don't want to do any more
        // updates or flooding - we simply want to return to the beginning for victory.
        if (allWallsInspected) {
            destroyStack(unexplored);
            break;
        }

        // If prev == NULL we're at the start, so no need to move
        if (target->m_prev != NULL) {

            // Once we've found a valid, non-origin target (i.e. one that has not
            // had all of its walls already inspected), we move to the proper
            // advancing position for that particular Cell. That is, we keep
            // backtracking until we're one away from that Cell, or until a 
            // request is made (at which point we immediately honor that request).

            while (target->m_prev != &m_cells[m_x][m_y]
                   && !(_undoRequested() || _resetRequested()) ) {

                // If at any point the target is one cell away, we've no need to
                // actually retrace though all of the other prev Cells. Thus we
                // can simply break to reduce the amount of retracing that we do

                if (isOneCellAway(target)) {
                    break;
                }

                // Similarly, if the staging Cell (i.e., the Cell that the mouse has to
                // travel to before it can move to the target Cell) is one Cell away, 
                // we've no need to completely retrace though all of the other prev Cells.
                // We can simply move to the staging cell and proceed.

                if (isOneCellAway(target->m_prev)) {
                    moveOneCell(target->m_prev);
                    break;
                }

                // At this point, we know that the mouse has to be *at least* two cells away
                // from the staging Cell. Thus, at this point, we check to see if we can move
                // to the staging Cell in a more efficient manner; in other words, we search
                // for a more efficient traceback path. The only inefficiencies (at this point)
                // could be due to a cell that has been fully explored but not yet traversed.
                // In that case, the mouse will *never* trace back through that square by
                // default, even if it'd more efficient. Thus we check all untraversed cells
                // for shorter paths

                if (tryUntraversed(target)) {
                    // We continue (as opposed to breaking) because we aren't sure if we're
                    // at the staging cell yet - we simply know that we've taken the most
                    // efficient shortcut through an untraversed Cell. Thus we must *still*
                    // retrace to get to the staging cell.
                    continue;
                }

                // At this point, we can't take any shortcuts in the retracing process.
                // Thus we simply find the previous Cell and move to it.
                struct Cell *prev = m_cells[m_x][m_y].m_prev;
                moveOneCell(prev);
            }

            // Once the mouse is in proper advancing position, advance. However, if
            // a request was made, we broke out of the while-loop and thus cannot
            // be guaranteed that we're one Cell away from the target. Thus we
            // only move if no requests were made.
            if (!(_undoRequested() || _resetRequested())) {
                moveOneCell(target);
            }
        }

        // Once we're at the cell, perform the appropriate updates for that cell,
        // including modifying the explore stack. In the case that a request was
        // made (and therefore cannot be guaranteed that we're at the target cell),
        // updating does not have any effect since we're going to be undo that last
        // update in our short term memory (so long as our short term memory > 0).
        doUpdatesForCurrentCell(unexplored);

        // Check to see if we've reached the center
        if (!m_centerReached && inGoal(m_x, m_y)) {
            m_centerReached = true;
            if (ALGO_COMPARE) {
                printf("Extensive center in %d steps\n",m_steps);
            }
        }

    }

    destroyStack(unexplored);

    // Once the stack is empty (once we've explored every possible cell),
    // we assign a maximum distance value to unexplored cells and then return
    // to the starting location. Thus, at the end of this function, all distance
    // values will be 100% complete and correct
    
    for (int x = 0; x < MAZE_SIZE_X; x++) {
        for (int y = 0; y < MAZE_SIZE_Y; y++) {
            if (!m_cells[x][y].m_explored) {
                // Any unreachable cells should have inf distance. Conveniently,
                // MAZE_SIZE_X*MAZE_SIZE_Y is one greater than the maximum distance
                m_cells[x][y].m_distance = MAZE_SIZE_X * MAZE_SIZE_Y;
            }
        }
    }

    // Mark the maze as fully explored. Note that we do this *prior* to attempting
    // to return to the beginning of the maze. The reason for this is that if we
    // make a mistake on our way back to the beginning, we can simply undo to
    // start from victory.
    m_explored = true;

    // Lastly, return to the starting location and face forward. Note that we can
    // use the same short-circuiting procedures here as we do when we backtrace.
    while (m_x != 0 || m_y != 0) {
        if (isOneCellAway(&m_cells[0][0])) {
            moveOneCell(&m_cells[0][0]);
        }
        else if (tryUntraversed(&m_cells[0][0])) {
            continue;
        }
        else {
            moveOneCell(m_cells[m_x][m_y].m_prev);
        }
    }
    if (m_d == 2) {
        turnAround();
    }
    else { // m_d == 3
        turnRight();
    }

}

void moveOneCell(struct Cell *target) {

    // Only move if the cell is actually one away
    if (isOneCellAway(target)) {

        // Use local variables to reduce function calls
        int x = target->m_x;
        int y = target->m_y;
        
        // Directional logic
        int absDir = 0; // Assume that the direction is upward
        if (x < m_x) {
            absDir = 3;        
        }
        else if (x > m_x) {
            absDir = 1;        
        }
        else if (y < m_y) {
            absDir = 2;        
        }

        // Turn the right direction
        if (absDir == m_d) {
            // Do nothing - we're already facing the right way
        }
        else if (absDir == (m_d + 1)%4) {
            turnRight();
        }
        else if (absDir == (m_d + 2)%4) {
            turnAround();
        }
        else if (absDir == (m_d + 3)%4) {
            turnLeft();
        }

        // Finally, move forward one space
        moveForward();

    } else {
        printf("ERROR: Tried to move to cell (%d,%d) but it's not one space away.\n",target->m_x,target->m_y);
    }

}

void doUpdatesForCurrentCell(struct CellStack *unexplored) {

    // List of cellmod objects for all cell modifications performed at this step
    struct List * modifiedCells = createList(); // List of Cellmod*

    // We need to keep track of the old values for the modified cell before we update it.
    appendModifiedCell(modifiedCells, &m_cells[m_x][m_y]);

    // Once we're in the correct Cells, we can now examine the contents of the
    // target and update our walls and distances
    walls();
    flood(m_x, m_y);

    // First we get a list of unexplored neighbors. Then we push the elements of this list
    // onto the unexplored stack in a particular order, depending on whether or not we made
    // it to the center already.
    struct List * unexploredNeighbors = createList(); // List of Cell*
    
    // Once we've examined the contents at the target, it is considered explored and traversed
    m_cells[m_x][m_y].m_explored = true;
    m_cells[m_x][m_y].m_traversed = true;

    // After, we find any unexplored neighbors. We use a front biased search since
    // this seems to perform better with dead-end detection.

    struct Cell *temp;
    temp = getLeftCell();

    if (!_wallLeft() && temp->m_prev == NULL) {

        // We need to keep track of the old values for the modified cell before we update it.
        appendModifiedCell(modifiedCells, temp);

        push(unexplored, temp);
        temp->m_prev = &m_cells[m_x][m_y];
    }
    temp = getFrontCell();
    if (!_wallFront() && temp->m_prev == NULL) {

        // We need to keep track of the old values for the modified cell before we update it.
        appendModifiedCell(modifiedCells, temp);

        push(unexplored, temp);
        temp->m_prev = &m_cells[m_x][m_y];
    }
    temp = getRightCell();
    if (!_wallRight() && temp->m_prev == NULL) {

        // We need to keep track of the old values for the modified cell before we update it.
        appendModifiedCell(modifiedCells, temp);

        push(unexplored, temp);
        temp->m_prev = &m_cells[m_x][m_y];
    }

    // If we've reached the center already, simply use F-R-L priority
    if (m_centerReached) {
        while (!isEmptyList(unexploredNeighbors)) {
            push(unexplored, front(unexploredNeighbors));
            pop_front(unexploredNeighbors);
        }
    }

    // If we haven't yet reached the center, push the cells onto the stack so that the
    // center-most cell is at the top. In this way, the mouse will prioritize the
    // center-most cells while it's still trying to find the center.

    else {
        while (!isEmptyList(unexploredNeighbors)) {

            struct Cell *leastCentered = NULL;
            int distFromCenter = -1;
            int relativeCenteredness = -1;

            // For each iteration of the loop, get the least center-most Cell and
            // push it to the unexplored stack

            struct ListNode * node = unexploredNeighbors->front;
            while (true) {
                
                int xDist = abs(MAZE_SIZE_X/2 - ((struct Cell *)node->data)->m_x);
                int yDist = abs(MAZE_SIZE_Y/2 - ((struct Cell *)node->data)->m_y);

                if ((xDist + yDist) > distFromCenter) {
                    leastCentered = node->data;
                    distFromCenter = (xDist + yDist);
                    relativeCenteredness = abs(xDist - yDist);
                }
                else if ((xDist + yDist) == distFromCenter && abs(xDist - yDist) > relativeCenteredness) {
                    leastCentered = node->data;
                    distFromCenter = (xDist + yDist);
                    relativeCenteredness = abs(xDist - yDist);
                }

                if (node == unexploredNeighbors->back) {
                    break;
                }
                node = node->behind;
            }

            push(unexplored, leastCentered);
            removeItem(unexploredNeighbors, leastCentered);
        }
    }
    destroyList(unexploredNeighbors);

    // Update the History target stack and modified cells
    stackUpdate(&m_history, unexplored);
    modifiedCellsUpdate(&m_history, modifiedCells);
    destroyList(modifiedCells,true);
}

bool isOneCellAway(struct Cell *target) {

    // Use local variables to reduce function calls
    int x = target->m_x;
    int y = target->m_y;
    
    if ((m_x == x) && (m_y + 1 == y) && !m_cells[m_x][m_y].m_walls[NORTH]) {
        return true;
    }
    else if ((m_x == x) && (m_y - 1 == y) && !m_cells[m_x][m_y].m_walls[SOUTH]) {
        return true;
    }
    else if ((m_x + 1 == x) && (m_y == y) && !m_cells[m_x][m_y].m_walls[EAST]) {
        return true;
    }
    else if ((m_x - 1 == x) && (m_y == y) && !m_cells[m_x][m_y].m_walls[WEST]) {
        return true;
    }
    
    return false;
}

// Attempts to short-circuit the retracing by looking at neighbors of untraversed cells
// Returns true if successful, false if short-circuiting wasn't possible
bool tryUntraversed(struct Cell *target) {

    int dir = -1; // 0: front, 1: right, 3:left (rear cannot be untraversed)
    int mostCount = -1; // Keeps track of highest number of short-circuited steps

    // First, find the direction with the greatest number of short circuited steps

    for (int i = 0; i < 3; i++) {

        struct Cell *neighbor = NULL;
        switch(i) {
            case 0:
                neighbor = getFrontCell();
                break;
            case 1:
                neighbor = getRightCell();
                break;
            case 2:
                neighbor = getLeftCell();
                break;
        }

        // Check to make sure that the cell is reachable (i.e., that there is not all
        // wall in between) and that it is not traverse (since only untraversed cells
        // are of interest to us.
        if (isOneCellAway(neighbor) && !neighbor->m_traversed) {

            // Find the cell that discovered the untraversed cell. Then check to see if *that*
            // cell is on the retracing path. If so, then we can short circuiting the retracing
            struct Cell *neighborPrev = neighbor->m_prev;

            // Sanity check
            if (neighborPrev != NULL) {

                // This pointer will trace back along the normal retracing path, checking to see
                // if neighborPrev is on the path (and more than one step away)
                struct Cell *runner = &m_cells[m_x][m_y];

                int counter = 0; // Counts how many steps that the mouse would have to retract
                                 // without short circuiting via the untraversed cell. If the
                                 // value of count ends up being more than one, we know that
                                 // it is *always* in our favor to retrace. Note that this also
                                 // helps us to choose the most profitable direction to move in
                                 // if multiple directions are untraversed.

                // This loop guarentees that we check for short circuiting up until the
                // prev Cell for the target. However, we need to check target->m_prev
                // explicitly afterwards
                while (runner != target->m_prev) {

                    if (neighborPrev == runner) {
                        if (dir == -1 || counter < mostCount) {
                            dir = i;
                            mostCount = counter;
                        }
                        break; // Break out of the while-loop
                    }

                    counter++;
                    runner = runner->m_prev;
                }
                
                // At this point, we're guarenteed that runner == target->m_prev
                if (neighborPrev == runner) {
                    if (dir == -1 || counter < mostCount) {
                        dir = i;
                        mostCount = counter;
                    }
                }
            }
        }

        // Continue with the next iteration of the for-loop i.e., examine the next untraversed cell
    }

    // At this point, dir represents the direction with the greatest short circuiting
    // or -1 if no directions are able to short circuit. Thus, we can either move to
    // the short circuiting cell (and return true) or not (and return false). The
    // condition that mostCount > 1, as opposed to mostCount > 2, causes the mouse to
    // prefer taking untraversed paths as opposed to already traversed paths in the
    // case when mostCount is 2. In a few situations, this will cause the mouse to 
    // take *slightly* more steps, but the advantage of actually traversing a cell as
    // opposed to simply observing cell wall values is worth the trade-off.

    if (dir >= 0 && mostCount > 1) {
        struct Cell *untraversed = NULL; 
        switch(dir) {
            case 0:
                untraversed = getFrontCell();
                break;
            case 1:
                untraversed = getRightCell();
                break;
            case 2:
                untraversed = getLeftCell();
                break;
        }
        moveOneCell(untraversed);
        moveOneCell(untraversed->m_prev);
        return true;
    }
    return false;

}

void bffExplore(struct CellStack *path) {

    // The first checkpoint is at the origin
    struct Cell *cpCell = &m_cells[0][0];

    // A queue of lists of modified cells
    struct List * modCells = createList(); // queue of list of SimpleCellmod

    while (true) {

        // The first thing we do is return to a checkpoint if need be
        while (!m_checkpointReached) {

            // Get the path to the checkpoint
            struct Cell *cpPathRunner = cpCell;
            struct CellStack *cpPath = createStack();
            while (cpPathRunner != NULL) {
                push(cpPath, cpPathRunner);
                cpPathRunner = cpPathRunner->m_prev;
            }
            pop(cpPath); // Pop off (0,0)

            // Return to the checkpoint
            while (!isEmpty(cpPath) && !(_undoRequested() || _resetRequested())) {
                moveOneCell(top(cpPath));
                pop(cpPath);
            }
            destroyStack(cpPath);
            
            // Check for requests during our return to checkpoint
            if (_resetRequested()) {
                _resetPosition();
                _resetHonored();
                return;
            }
            if (_undoRequested()) {
                m_x = 0;
                m_y = 0;
                m_d = 0;
                _resetPosition();
                _undoHonored();
                continue;
            }

            // If no requests were made, we've reached the checkpoint
            m_checkpointReached = true;
        }

        // Solve the maze using basic floodfill
        while (!inGoal(m_x, m_y) && !(_undoRequested() || _resetRequested())) {

            // List of modified cells for this step
            struct List * modCellsList = createList();

            // Always add the current cell to modified cells
            bffAppendModifiedCell(modCellsList, &m_cells[m_x][m_y]);

            // Keep track of wall updates
            if (spaceLeft()) {
                bffAppendModifiedCell(modCellsList, getLeftCell());
            }
            if (spaceFront()) {
                bffAppendModifiedCell(modCellsList, getFrontCell());
            }
            if (spaceRight()) {
                bffAppendModifiedCell(modCellsList, getRightCell());
            }

            // Do all Cell updates
            walls();
            flood(m_x, m_y);
            dobffCellUpdates();
            moveTowardsGoal();

            // Push the old states of the updated cells to modCells
            push_back(modCells, copyOfList(modCellsList));

            // Make sure we're only keeping short term history
            if (size(modCells) > SHORT_TERM_MEM) {
                struct List * temp = modCells->front->data;
                pop_front(modCells);
                destroyList(temp);
            }

            destroyList(modCellsList);
        }

        // NICE!
        walls();
        flood(m_x,m_y);

        // If there were requests during the solve, honor them now
        if (_resetRequested()) {
            _resetPosition();
            _resetHonored();
            return;
        }

        if (_undoRequested()) {

            if (size(modCells) == SHORT_TERM_MEM) {
                cpCell = ((struct SimpleCellmod *)front(front(modCells)))->cell;

                // Since we don't update after reaching the checkpoint, don't undo mods at checkpoint
                struct List * temp22 = modCells->front->data; // TODO: Bad.
                pop_front(modCells);
                destroyList(temp22);

                // Iterate through all modified cells, starting with most recent and going
                // to least recent. During iterations, we simply restore the old values
                while (!isEmptyList(modCells)) {

                    struct List * cellList = copyOfList(front(modCells)); // List of SimpleCellmod

                    struct List * temp23 = modCells->front->data;
                    pop_front(modCells);
                    destroyList(temp23);

                    struct ListNode * node = cellList->front;
                    while (true) {
                        //((struct SimpleCellmod *)(node->data))->cell->m_prev = ((struct SimpleCellmod)(node->data)).prev;
                        //((struct SimpleCellmod *)(node->data))->cell->m_distance = node->data.dist;
                        node->data.cell->m_prev = node->data.prev;
                        node->data.cell->m_distance = node->data.dist;
                        for (int i = 0; i < 4; i++) {
                            node->data.cell->m_walls[i] = node->data.walls[i];
                        }
                        if (node == cellList->back) {
                            break;
                        }
                        node = node->behind;
                    }
                }
            }
            m_x = 0; // Since we're repositioning the mouse but not re-intializing the
            m_y = 0; // maze, we have to explicitely reset the x, y, and d values
            m_d = 0;
            m_checkpointReached = false;
            _resetPosition();
            _undoHonored();
            continue;
        }


        destroyList(modCells);

        // Note: As it stands right now, if the mouse screws up going into the final few cells of
        // the maze and we don't hit reset before it thinks it solved the maze, we don't really
        // have a good way to undo the changes.

        if (ALGO_COMPARE) {
            printf("Simple center in %d steps\n",m_steps);
        }

        // Populate the path variable with the actual path of the robot to the center. We do
        // this rather than simply returning a stack since the mouse may screw up on it's
        // way back to the origin, but we still want to be able to retain the floodfill info.
        // In retaining this info, we have to mark m_explored as true. But this can't be done
        // until we have the best path. Hence the return parameter.
        struct Cell *runner = &m_cells[m_x][m_y];
        while (runner->m_prev != NULL) {
            push(path, runner);
            runner = runner->m_prev;
        }

        // Indicate that the solve is done
        m_explored = true;

        // Return to start
        while ((m_x != 0 || m_y != 0) && !(_undoRequested() || _resetRequested())) {
            if (isOneCellAway(&m_cells[0][0])) {
                moveOneCell(&m_cells[0][0]);
            }
            else if (tryUntraversed(&m_cells[0][0])) {
                continue;
            }
            else {
                moveOneCell(m_cells[m_x][m_y].m_prev);
            }
        }
        if (!(_undoRequested() || _resetRequested())) {
            if (m_d == 2) {
                turnAround();
            }
            else { // m_d == 3
                turnRight();
            }
        }
        return; // We're done
        
    }

}

void dobffCellUpdates() {

    // Note: This could be improved so that prev values are updated when we find prev cells
    // that are closer to the origin - without this, the algo will always (naively) take
    // the same path, even if it's much less efficient to do so.

    struct Cell *temp;

    temp = getLeftCell();
    if (!_wallLeft() && temp->m_prev == NULL && temp != &m_cells[0][0]) {
        temp->m_prev = &m_cells[m_x][m_y];
    }
    temp = getFrontCell();
    if (!_wallFront() && temp->m_prev == NULL && temp != &m_cells[0][0]) {
        temp->m_prev = &m_cells[m_x][m_y];
    }
    temp = getRightCell();
    if (!_wallRight() && temp->m_prev == NULL && temp != &m_cells[0][0]) {
        temp->m_prev = &m_cells[m_x][m_y];
    }

}

void bffVictory(struct CellStack *path) {

    while (true) {

        struct CellStack *copy = copyOfStack(path);

        // Move to the center of the maze along the path
        while (!isEmpty(copy) && !(_undoRequested() || _resetRequested())) {
            moveOneCell(top(copy)); 
            pop(copy);
        }

        // Return to start
        while ((m_x != 0 || m_y != 0) && !(_undoRequested() || _resetRequested())) {
            moveOneCell(m_cells[m_x][m_y].m_prev);
        }
        if (!(_undoRequested() || _resetRequested())) {
            if (m_d == 2) {
                turnAround();
            } else { // m_d == 3
                turnRight();
            }
        }

        destroyStack(copy);

        if (_resetRequested()) {
            // This return will cause bffvictory() to exit, and thus
            // we reinitialize the maze and start our solve over
            _resetPosition();
            _resetHonored();
            return;
        }
        if (_undoRequested()) {
            m_x = 0; // Since we're repositioning the mouse but not re-intializing the
            m_y = 0; // maze, we have to explicitely reset the x, y, and d values
            m_d = 0;
            _resetPosition();
            _undoHonored();
        }

        if (ALGO_COMPARE) {
            return;
        }
    }

}

bool proceedToCheckpoint(struct CellStack *path) {

    while (!isEmpty(path)) {

        // On each iteration, first check for requests
        if (_resetRequested() || _undoRequested()) {
            return true;
        }

        struct Cell *next = top(path);
        pop(path);

        // Since we popped the origin off of the path stack, we're guaranteed that we
        // won't try to moveOneCell() to the origin
        moveOneCell(next);
    }

    return false;

}

void bffAppendModifiedCell(struct List * modCellsList, struct Cell *cell) { // List of simplecellmod
    push_back(modCellsList, createSimpleCellmod(cell,cell->m_prev,cell->m_distance,cell->m_walls));
}

void appendModifiedCell(struct List *modList, struct Cell *modCell) { // List of cellmod

    push_back(modList, createCellmod(modCell,modCell->m_prev,modCell->m_distance,modCell->m_explored,modCell->m_traversed,
                                     modCell->m_walls,modCell->m_wallsInspected));

}

bool checkRequestVictory() {

    // If a request has been made, perform appropriately. Since the maze has already
    // been completely solved, on reset we start over and on undo we simply retry
    // solving the maze as quickly as possible (without changing wall information)
    if (_undoRequested() || _resetRequested()) {
        _resetPosition();
        if (_resetRequested()) {
            // This return will cause victory() to exit, and thus
            // we reinitialize the maze and start our solve over
            _resetHonored();
            return true;
        }
        else {
            m_x = 0; // Since we're repositioning the mouse but not re-intializing the
            m_y = 0; // maze, we have to explicitely reset the x, y, and d values
            m_d = 0;
            _undoHonored();
        }
    }
    return false;

}
