#include "Cell.h"
#include "CellStack.h"
#include "FloodFill.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main(void) { solve(); }

void solve(void) { // This is what gets called from outside of this file.

    m_x = 0; // Initialize the x position of the mouse
    m_y = 0; // Initialize the y position of the mouse
    m_d = 0; // Initialize the direction position of the mouse
    m_steps = 0; // Initialize the mouse steps

    // Initialize the x and y positions of the cells
    for (int x = 0; x < X_WIDTH; x++) {
        for (int y = 0; y < Y_WIDTH; y++) {
            m_cells[x][y].m_x = x;
            m_cells[x][y].m_y = y;
        }
    }

    // Initialize the Cells.
    initializeCells();

    // Augmented Floodfill - Explore the maze to its entirety, returning to the start
    explore();
    printf("%d\n", m_steps); // Print the total number of steps

    struct CellStack *optimalPath = newStack();

    // Loop forever, continue going to the beginning and solving
    while (true) {

        // Solve the maze
        while (!inGoal()) {
            push(optimalPath, &m_cells[m_x][m_y]);
            moveTowardsGoal();
        }

        // Return to start
        while (m_x != 0 || m_y != 0) {
            struct Cell *prev = top(optimalPath);
            pop(optimalPath);
            moveOneCell(prev);
        }

        emptyStack(optimalPath);

    }

    destroyStack(optimalPath); // We won't reach this point, but I feel like it's good coding practice.

}

void printDistances(void) {

    printf("\n");

    for (int y = Y_WIDTH-1; y >= 0; y--) {
        for (int x = 0; x < X_WIDTH; x++) {
            if (m_cells[x][y].m_distance < 100) {
                if (m_cells[x][y].m_distance < 10) {
                    printf(" ");
                }
                printf(" ");
            }
            printf("%d ", m_cells[x][y].m_distance);
        }
        printf("\n");
    }

}

void printWalls(void) {

    printf("\n");

    for (int y = Y_WIDTH-1; y >= 0; y--) {
        for (int x = 0; x < X_WIDTH; x++) {
            int wallSum = 0;
            wallSum += 1*m_cells[x][y].m_walls[NORTH];
            wallSum += 2*m_cells[x][y].m_walls[EAST];
            wallSum += 4*m_cells[x][y].m_walls[SOUTH];
            wallSum += 8*m_cells[x][y].m_walls[WEST];
            if (wallSum < 10) {
                printf(" ");
            }
            printf("%d ", wallSum);
        }
        printf("\n");
    }

}

void initializeCells(void) {

    // NOTE: Doesn't work for odd sized mazes. // TODO: But this is OK, right, since odd sized mazes don't have a 2x2 center?

    // Initialize the distance values for the maze
    for (int y = 0; y < Y_WIDTH/2; y++) {
        int distance = X_WIDTH/2 + Y_WIDTH/2 - 2 - y;
        for(int x = 0; x < X_WIDTH/2; x++) {
            m_cells[x][y].m_distance = distance; // Bottom-left
            m_cells[x][Y_WIDTH-1-y].m_distance = distance; // Upper-left
            m_cells[X_WIDTH-1-x][Y_WIDTH-1-y].m_distance = distance; // Upper-right
            m_cells[X_WIDTH-1-x][y].m_distance = distance; // Bottom-right
            distance -= 1;
        }
    }

    // Initialize other values for the Cells
    for (int y = 0; y < Y_WIDTH; y++) {
        for (int x = 0; x < X_WIDTH; x++) {

            // Set the walls values
            m_cells[x][y].m_walls[SOUTH] = (y == 0);
            m_cells[x][y].m_walls[NORTH] = (y == Y_WIDTH-1);
            m_cells[x][y].m_walls[WEST] = (x == 0);
            m_cells[x][y].m_walls[EAST] = (x == X_WIDTH-1);

            // Set the inspected values of the walls
            m_cells[x][y].m_wallsInspected[SOUTH] = (y == 0);
            m_cells[x][y].m_wallsInspected[NORTH] = (y == Y_WIDTH-1);
            m_cells[x][y].m_wallsInspected[WEST] = (x == 0);
            m_cells[x][y].m_wallsInspected[EAST] = (x == X_WIDTH-1);

            // Set the prev and explored values of the Cells
            m_cells[x][y].m_prev = NULL;
            m_cells[x][y].m_explored = false;
        }
    }

}

void walls(void) {

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
    }

    if (spaceLeft()) {
        temp = getLeftCell();
        temp->m_walls[(m_d+1)%4] = _wallLeft();
        temp->m_wallsInspected[(m_d+1)%4] = true;
    }

    if (spaceRight()) {
        temp = getRightCell();
        temp->m_walls[(m_d+3)%4] = _wallRight();
        temp->m_wallsInspected[(m_d+3)%4] = true;
    }

}

void flood(int x, int y) {
    
    // *DO NOT* flood the maze if the mouse is in the goal - there is no
    // information that we can gain about the distance values in the goal,
    // and we can only corrupt the current distance values anyways
    if (!inGoal()) {

        // Initialize distance values for surrounding cells
        int northDistance = X_WIDTH*Y_WIDTH; // Max distance
        int eastDistance = X_WIDTH*Y_WIDTH; // Max distance
        int southDistance = X_WIDTH*Y_WIDTH; // Max distance
        int westDistance = X_WIDTH*Y_WIDTH; // Max distance

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

void moveTowardsGoal(void) {
    
    // Invariant: One of the adjacent cells is guarenteed to have a lower value, so
    //            it's safe to initialize the minimum distance to current distance
    int minDistance = m_cells[m_x][m_y].m_distance; // Initialize distance
    int relDirection = 0; // Initialize direction
    struct Cell *temp;

    if (!m_cells[m_x][m_y].m_walls[(m_d+3)%4]) { // Check left
        temp = getLeftCell();
        if (temp->m_distance <= minDistance) {
            minDistance = temp->m_distance;
            relDirection = 3;
        }
    }

    if (!m_cells[m_x][m_y].m_walls[(m_d+2)%4]) { // Check behind
        temp = getRearCell();
        if (temp->m_distance <= minDistance) {
            minDistance = temp->m_distance;
            relDirection = 2;
        }
    }

    if (!m_cells[m_x][m_y].m_walls[(m_d+1)%4]) { // Check right
        temp = getRightCell();
        if (temp->m_distance <= minDistance) {
            minDistance = temp->m_distance;
            relDirection = 1;
        }
    }

    if (!m_cells[m_x][m_y].m_walls[m_d]) { // Check front
        temp = getFrontCell();
        if (temp->m_distance <= minDistance) {
            minDistance = temp->m_distance;
            relDirection = 0;
        }
    }
    
    switch (relDirection) {
        case 1: // turn right
            turnRight();
            break;
        case 2: // turn around
            turnRight();
            turnRight();
            break;
        case 3: // turn left
            turnLeft();
            break;
    }

    moveForward(); // Move the robot forward

}

void moveForward(void) {

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

    // Then, set the new Cell's explored flag to true
    m_cells[m_x][m_y].m_explored = true;

    // Actually move the mouse forward in the simulation
    _moveForward();

    // Lastly, increment the number of steps
    m_steps++;

}

void turnRight(void) {

    m_d = (m_d + 1) % 4; // Update internal representation
    _turnRight(); // Move the mouse
    m_steps++; // Lastly, increment the number of steps

}

void turnLeft(void) {

    m_d = (m_d + 3) % 4; // Update internal representation
    _turnLeft(); // Move the mouse
    m_steps++; // Lastly, increment the number of steps

}

bool inGoal(void) {

    // The goal is defined to be the center of the maze 
    // This means that it's 4 squares of length if even, 1 if odd
    
    bool horizontal = (X_WIDTH - 1) / 2 == m_x;
    if (X_WIDTH % 2 == 0) {
        horizontal = horizontal || (X_WIDTH) / 2 == m_x;
    }

    bool vertical = (Y_WIDTH - 1) / 2 == m_y;
    if (Y_WIDTH % 2 == 0) {
        vertical = vertical || (Y_WIDTH) / 2 == m_y;
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
    } else {
        return secondMin;
    }

}

struct Cell * getFrontCell(void) {

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

    return NULL; // will not reach

}

struct Cell * getLeftCell(void) {

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

    return NULL; // will not reach

}

struct Cell * getRightCell(void) {

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

    return NULL; // will not reach

}

struct Cell * getRearCell(void) {

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

    return NULL; // will not reach

}

bool spaceFront(void) {

    switch (m_d) {
        case NORTH:
            return m_y+1 < Y_WIDTH;
        case EAST:
            return m_x+1 < X_WIDTH;
        case SOUTH:
            return m_y > 0;
        case WEST:
            return m_x > 0;
    }

    return false; // will not reach

}

bool spaceLeft(void) {

    switch (m_d) {
        case NORTH:
            return m_x > 0;
        case EAST:
            return m_y+1 < Y_WIDTH;
        case SOUTH:
            return m_x+1 < X_WIDTH;
        case WEST:
            return m_y > 0;
    }

    return false; // will not reach
}

bool spaceRight(void) {

    switch (m_d) {
        case NORTH:
            return m_x+1 < X_WIDTH;
        case EAST:
            return m_y > 0;
        case SOUTH:
            return m_x > 0;
        case WEST:
            return m_y+1 < Y_WIDTH;
    }

    return false; // will not reach

}


void explore(void) {

    /*
        The foundation of the explore method is a simple DFS search. However,
        DFS is not terribly efficient in this context and can be improved upon
        greatly. See below for improvements, failures and ideas. Any testing or
        comparison can be done with the exploreBeta method. That is, DO NOT
        edit this function directly. First, edit Beta. Once an improvement has
        been tested, it can be copied into this function.

        IMPROVEMENTS:
        1.) When tracing back to the next parent node with neighbors, check to
            see if, at any point in the retracing, that you're next to the
            target node. If so, there is no need to retrace all the way back to
            the parent node. Rather, we can simply proceed to the target node
            from where we currently are.
        2.) Keep track of which walls have been inspected. If all walls of a Cell
            have been inspected, there is no need to explicitly move the mouse
            into that particular Cell. Thus, after popping a Cell off of the stack,
            check to see if all of its walls have been explored. If so, pop the
            next Cell off of the stack and proceed. 

        FAILURES:
        1.) Updating the prev values as cells were rediscovered. This performed
            worse in every test case
        2.) Choosing to explore the neighbor with the lowest cell value, as
            opposed to a R-M-L priority (or L-M-R priority)
        3.) Choosing to explore the neighbor with the highest cell value, as
            opposed to a R-M-L priority (or L-M-R priority)
        4.) Choosing to explore the neighbor with a different directional
            priority, other than L-M-R or R-M-L

        IDEAS:
        1.) Do we need to explore cells that are far away from the goal? - Doing right now
        2.) If we decide that we don't need to explore a particular Cell, sometimes that cell
            is actually within the shortest path to the Cell to which we wish to backtrace - 
            remember the 6 cell example? How can we solve that...?
    */

    // Push unexplored nodes onto a stack
    struct CellStack *unexplored = newStack();

    // The initial square is at (0, 0)
    push(unexplored, &m_cells[0][0]);

    // Loop until all cells have been explored
    while (!isEmpty(unexplored)) {

        // Begin by assuming that all walls of the Cell that we intend to explore
        // have already been inspected. Thus, we assume that we have no need to
        // actually explore that Cell. We check the wallInspected values of each
        // of the walls of that Cell to confirm (or refute) our assumption. Once
        // we find a Cell for which not all of the walls have been inspected, we
        // proceed as normal.

        bool allWallsInspected = true;
        struct Cell *target = NULL; // Initialze the target cell

        while (allWallsInspected && !isEmpty(unexplored)) {

            // First, pop the target cell off of the stack
            target = top(unexplored); 
            pop(unexplored);
    
            // Next, check to see if all of the walls have been inspected.
            // If so, no need to explore that cell - get a new target
            for (int i = 0; i < 4; i++) {
                if (!target->m_wallsInspected[i]) {
                    allWallsInspected = false;
                    break;
                }
            } 
            
            // Although we don't need to explicitly explore a Cell if we've
            // already inspected all of its wall values, we still need to
            // flood it (and its neighbors) with a proper distance value.
            // In addition, we need to mark it as explored, acknowledging
            // that once we've inspected all walls, we've essentially explored
            // that Cell. If we don't mark the Cell as explored, then we may
            // accidentally assign it a maximum distance value by accident,
            // since we assign these values based on whether or not a Cell was
            // explored or not. Note that there is no danger of setting an
            // unreachable Cell as explored, since an unreachable cell will
            // be pushed onto the stack in the first place
            if (allWallsInspected) {
                target->m_explored = true;
                flood(target->m_x, target->m_y);
            }
        }
    
        // Once we've found a valid target (i.e. one that has not had all of
        // its walls already inspected), we move to the proper advancing
        // position for that particular Cell. That is, we keep backtracking
        // until we're one away from that Cell.

        if (target->m_prev != NULL) { // If prev == NULL we're at the start, so no need to move

            // While the mouse is not in the advancing position, trace back
            while (target->m_prev != &m_cells[m_x][m_y]) {

                // If at any point the target is one cell away, we've no need to
                // actually retrace though all of the other prev Cells. Thus we
                // can simply break to reduce the amount of retracing that we do

                if (isOneCellAway(target)) {
                    break;
                }

                struct Cell *prev = m_cells[m_x][m_y].m_prev;
                moveOneCell(prev);
            }

            // Once the mouse is in proper advancing position, advance
            moveOneCell(target);
        }

        // Once we're in the correct Cells, we can now examine the contents of the
        // target and update our walls and distances
        walls();
        flood(m_x, m_y);

        // Once we've examined the contents at the target, it is considered explored
        m_cells[m_x][m_y].m_explored = true;
        
        // After, we find any unexplored neighbors

        struct Cell *temp = getLeftCell();
        if (!_wallLeft() && temp->m_prev == NULL) {
            push(unexplored,temp);
            temp->m_prev = &m_cells[m_x][m_y];
        }

        temp = getFrontCell();
        if (!_wallFront() && temp->m_prev == NULL) {
            push(unexplored,temp);
            temp->m_prev = &m_cells[m_x][m_y];
        }

        temp = getRightCell();
        if (!_wallRight() && temp->m_prev == NULL) {
            push(unexplored,temp);
            temp->m_prev = &m_cells[m_x][m_y];
        }

    }

    destroyStack(unexplored);

    // Once the stack is empty (once we've explored every possible cell),
    // we assign a maximum distance value to unexplored cells and then return
    // to the starting location. Thus, at the end of this function, all distance
    // values will be 100% complete and correct
    
    for (int x = 0; x < X_WIDTH; x++) {
        for (int y = 0; y < Y_WIDTH; y++) {
            if (!m_cells[x][y].m_explored) {
                // Any unreachable cells should have inf distance. Conveniently,
                // X_WIDTH*Y_WIDTH is slightly greater than the maximum distance
                m_cells[x][y].m_distance = X_WIDTH*Y_WIDTH;
            }
        }
    }

    // Lastly, return to the starting location and face forward
    while (m_x != 0 || m_y != 0) {
        struct Cell *prev = m_cells[m_x][m_y].m_prev;
        moveOneCell(prev);
    }
    while (m_d != 0) { 
        turnRight(); // Turning right is optimal since we'd never 
                     // approach the starting location from the left
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
            turnRight();
            turnRight();
        }
        else if (absDir == (m_d + 3)%4) {
            turnLeft();
        }

        // Finally, move forward one space
        moveForward();
    }
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

/* Not being used right now.
void basicFloodFill(void) {

    // Regular Floodfill Algorithm
    while (!inGoal()) {
        walls();
        flood(m_x, m_y);
        moveTowardsGoal();
    }

}
*/
