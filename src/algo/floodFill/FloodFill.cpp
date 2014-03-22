#include "FloodFill.h"

#include <iostream>
#include <stack>
#include <queue>
#include <utility>
#include <vector>

#include "Cellmod.h"

void FloodFill::solve(sim::MouseInterface* mouse){

    // Initialize the mouse pointer
    m_mouse = mouse;

    // Initialize the x and y positions of the cells
    for (int x = 0; x < MAZE_SIZE; x++){
        for (int y = 0; y < MAZE_SIZE; y++){
            m_cells[x][y].setX(x);
            m_cells[x][y].setY(y);
        }
    }

    // Loop to allow for reset requests to be honored
    while (true){

        // Initialize all mutable fields, including the Cells' walls
        // and distances as well as the mouse-related fields
        initialize();

        // Augmented DFS - Explore the maze to its entirety, returning to the start
        explore();

        // If the explore was not successful (i.e., reset request was made) then start over
        if (!m_explored) {
            continue;
        }

        // Once we know everything about the maze, solve the maze as quickly as possible,
        victory();
    }
}

void FloodFill::printDistances(){
    std::cout << std::endl;
    for (int y = MAZE_SIZE-1; y >= 0; y--){
        for (int x = 0; x < MAZE_SIZE; x++){
            if (m_cells[x][y].getDistance() < 100){
                if (m_cells[x][y].getDistance() < 10){
                    std::cout << " ";
                }
                std::cout << " ";
            }
            std::cout << m_cells[x][y].getDistance() << " ";
        }
        std::cout << std::endl;
    }
}

void FloodFill::printWalls(){
    std::cout << std::endl;
    for (int y = MAZE_SIZE-1; y >= 0; y--){
        for (int x = 0; x < MAZE_SIZE; x++){
            int wallSum = 0;
            wallSum += 1*m_cells[x][y].isWall(NORTH);
            wallSum += 2*m_cells[x][y].isWall(EAST);
            wallSum += 4*m_cells[x][y].isWall(SOUTH);
            wallSum += 8*m_cells[x][y].isWall(WEST);
            if (wallSum < 10){
                std::cout << " ";
            }
            std::cout << wallSum << " ";
        }
        std::cout << std::endl;
    }
}

void FloodFill::resetColors(){
    m_mouse->resetColors(m_x, m_y);
}

void FloodFill::initialize(){

    // TODO: Doesn't work for odd sized mazes

    // Initialize the distance values for the maze
    int distance = MAZE_SIZE - 2;
    for (int y = 0; y < MAZE_SIZE/2; y++){
        for(int x = 0; x < MAZE_SIZE/2; x++){
            m_cells[x][y].setDistance(distance); // Bottom-left
            m_cells[y][MAZE_SIZE-1-x].setDistance(distance); // Upper-left
            m_cells[MAZE_SIZE-1-x][MAZE_SIZE-1-y].setDistance(distance); // Upper-right
            m_cells[MAZE_SIZE-1-y][x].setDistance(distance); // Bottom right
            distance -= 1;
        }
        distance += (MAZE_SIZE/2 - 1);
    }

    // Initialize other values for the Cells
    for (int y = 0; y < MAZE_SIZE; y++){
        for (int x = 0; x < MAZE_SIZE; x++){

            // Set the walls values
            m_cells[x][y].setWall(SOUTH, y == 0);
            m_cells[x][y].setWall(NORTH, y == MAZE_SIZE-1);
            m_cells[x][y].setWall(WEST, x == 0);
            m_cells[x][y].setWall(EAST, x == MAZE_SIZE-1);

            // Set the inspected values of the walls
            m_cells[x][y].setWallInspected(SOUTH, y == 0);
            m_cells[x][y].setWallInspected(NORTH, y == MAZE_SIZE-1);
            m_cells[x][y].setWallInspected(WEST, x == 0);
            m_cells[x][y].setWallInspected(EAST, x == MAZE_SIZE-1);

            // Set the prev and explored values of the Cells
            m_cells[x][y].setPrev(NULL);
            m_cells[x][y].setExplored(false);
            m_cells[x][y].setTraversed(false);
        }
    }

    // Initialize mouse-related fields
    m_x = 0; // Initialize the x position of the mouse
    m_y = 0; // Initialize the y position of the mouse
    m_d = 0; // Initialize the direction position of the mouse
    m_steps = 0; // Initialize the mouse steps
    m_explored = false; // Initialize the exploredness of the maze
    m_checkpointReached = true; // We begin at the origin, the first checkpoint
    m_history.initialize(SHORT_TERM_MEM, &m_cells[0][0]); // Initialize the History object
}

void FloodFill::victory(){

    // TODO: Set the speed greater with each iteration
    // TODO: Give this to Tomasz in an array form

    // Loop forever, continue going to the beginning and solving
    while (true){

        std::stack<Cell*> optimalPath;

        // Solve the maze (while request hasn't been made)
        while (!inGoal(m_x, m_y) && !(m_mouse->undoRequested() || m_mouse->resetRequested())){
            optimalPath.push(&m_cells[m_x][m_y]);
            moveTowardsGoal();
        }

        // Return to start (while request hasn't been made)
        while ((m_x != 0 || m_y != 0) && !(m_mouse->undoRequested() || m_mouse->resetRequested())){
            Cell* prev = optimalPath.top();
            optimalPath.pop();
            moveOneCell(prev);
        }

        // If a reset request is made, return so we can re-init and re-explore
        if (checkRequestVictory()) {
            return;
        }
    }
}

void FloodFill::walls(){

    // Sets the wall values for the current cell
    m_cells[m_x][m_y].setWall(m_d, m_mouse->wallFront());
    m_cells[m_x][m_y].setWall((m_d+1)%4, m_mouse->wallRight());
    m_cells[m_x][m_y].setWall((m_d+3)%4, m_mouse->wallLeft());

    // Sets the wallInspected values for the current cell
    m_cells[m_x][m_y].setWallInspected(m_d, true);
    m_cells[m_x][m_y].setWallInspected((m_d+1)%4, true);
    m_cells[m_x][m_y].setWallInspected((m_d+3)%4, true);

    // Sets the wall and wallInspected values for the surrounding cells
    if (spaceFront()){
        getFrontCell()->setWall((m_d+2)%4, m_mouse->wallFront());
        getFrontCell()->setWallInspected((m_d+2)%4, true);
    }
    if (spaceLeft()){
        getLeftCell()->setWall((m_d+1)%4, m_mouse->wallLeft());
        getLeftCell()->setWallInspected((m_d+1)%4, true);
    }
    if (spaceRight()){
        getRightCell()->setWall((m_d+3)%4, m_mouse->wallRight());
        getRightCell()->setWallInspected((m_d+3)%4, true);
    }
}

void FloodFill::flood(int x, int y){
    
    // *DO NOT* flood the cells in the goal region of the maze- there is no
    // information that we can gain about the distance values in the goal,
    // and we can only corrupt the current distance values anyways (they
    // can't and shouldn't change from 0)
    if (!inGoal(x, y)){

        // Initialize distance values for surrounding cells
        int northDistance = MAZE_SIZE*MAZE_SIZE;
        int eastDistance = MAZE_SIZE*MAZE_SIZE;
        int southDistance = MAZE_SIZE*MAZE_SIZE;
        int westDistance = MAZE_SIZE*MAZE_SIZE;

        // Obtain actual values if possible
        if (!m_cells[x][y].isWall(NORTH)){
            northDistance = m_cells[x][y+1].getDistance();
        }
        if (!m_cells[x][y].isWall(EAST)){
            eastDistance = m_cells[x+1][y].getDistance();
        }
        if (!m_cells[x][y].isWall(SOUTH)){
            southDistance = m_cells[x][y-1].getDistance();
        }
        if (!m_cells[x][y].isWall(WEST)){
            westDistance = m_cells[x-1][y].getDistance();
        }

        // Check to see if the distance value is the min plus one
        if (m_cells[x][y].getDistance() != min(northDistance, eastDistance, southDistance, westDistance) + 1){

            // Set the value to the min plus one
            m_cells[x][y].setDistance(min(northDistance, eastDistance, southDistance, westDistance) + 1);

            if (!m_cells[x][y].isWall(NORTH)){
                flood(x, y + 1);
            }
            if (!m_cells[x][y].isWall(EAST)){
                flood(x + 1, y);
            }
            if (!m_cells[x][y].isWall(SOUTH)){
                flood(x, y - 1);
            }
            if (!m_cells[x][y].isWall(WEST)){
                flood(x - 1, y);
            }
        }
    }
}


void FloodFill::moveTowardsGoal(){
    
    // Invariant: One of the adjacent cells is guarenteed to have a lower value, so
    //            it's safe to initialize the minimum distance to current distance
    int minDistance = m_cells[m_x][m_y].getDistance(); // Initialize distance
    int relDirection = 0; // Initialize direction

    if (!m_cells[m_x][m_y].isWall((m_d+3)%4)){ // Check left
        if (getLeftCell()->getDistance() <= minDistance){
            minDistance = getLeftCell()->getDistance();
            relDirection = 3;
        }
    }
    if (!m_cells[m_x][m_y].isWall((m_d+2)%4)){ // Check behind
        if (getRearCell()->getDistance() <= minDistance){
            minDistance = getRearCell()->getDistance();
            relDirection = 2;
        }
    }
    if (!m_cells[m_x][m_y].isWall((m_d+1)%4)){ // Check right
        if (getRightCell()->getDistance() <= minDistance){
            minDistance = getRightCell()->getDistance();
            relDirection = 1;
        }
    }
    if (!m_cells[m_x][m_y].isWall(m_d)){ // Check front
        if (getFrontCell()->getDistance() <= minDistance){
            minDistance = getFrontCell()->getDistance();
            relDirection = 0;
        }
    }
    
    switch (relDirection){
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

void FloodFill::moveForward(){

    // NOTE: There is no preliminary check for walls or for limits in this
    //       function - this is the responsibility of the caller
    
    // First, update our internal representation of location
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

    // Actually move the mouse forward in the simulation
    m_mouse->moveForward();

    // Increment the number of steps
    m_steps++;

    // If we're still exploring append to explore path. Although it won't hurt
    // anything to append to the history after we're done exploring, but it
    // doesn't help and would just be wasting time. We include the condition
    // that the checkpoint must be reached so that we're not appending to
    // the path unnecessarily.
    if (!m_explored && m_checkpointReached) {
        m_history.moved();
    }
}

void FloodFill::turnRight(){
    m_d = (m_d + 1) % 4; // Update internal representation
    m_mouse->turnRight(); // Move the mouse
    m_steps++; // Lastly, increment the number of steps
}

void FloodFill::turnLeft(){
    m_d = (m_d + 3) % 4; // Update internal representation
    m_mouse->turnLeft(); // Move the mouse
    m_steps++; // Lastly, increment the number of steps
}

bool FloodFill::inGoal(int x, int y){

    // The goal is defined to be the center of the maze 
    // This means that it's 4 squares of length if even, 1 if odd
    
    bool horizontal = (MAZE_SIZE - 1) / 2 == x;
    if (MAZE_SIZE % 2 == 0){
        horizontal = horizontal || (MAZE_SIZE) / 2 == x;
    }

    bool vertical = (MAZE_SIZE - 1) / 2 == y;
    if (MAZE_SIZE % 2 == 0){
        vertical = vertical || (MAZE_SIZE) / 2 == y;
    }

    return horizontal && vertical;
}

int FloodFill::min(int one, int two, int three, int four){
    int firstMin = one;
    int secondMin = three;
    if (two <= one){
        firstMin = two;
    }
    if (four <= three){
        secondMin = four;
    }
    if (firstMin <= secondMin){
        return firstMin;
    }
    else{
        return secondMin;
    }
}

Cell* FloodFill::getFrontCell(){
    switch (m_d){
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

Cell* FloodFill::getLeftCell(){
    switch (m_d){
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

Cell* FloodFill::getRightCell(){
    switch (m_d){
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

Cell* FloodFill::getRearCell(){
    switch (m_d){
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

bool FloodFill::spaceFront(){
    switch (m_d){
        case NORTH:
            return m_y+1 < MAZE_SIZE;
        case EAST:
            return m_x+1 < MAZE_SIZE;
        case SOUTH:
            return m_y > 0;
        case WEST:
            return m_x > 0;
    }
}

bool FloodFill::spaceLeft(){
    switch (m_d){
        case NORTH:
            return m_x > 0;
        case EAST:
            return m_y+1 < MAZE_SIZE;
        case SOUTH:
            return m_x+1 < MAZE_SIZE;
        case WEST:
            return m_y > 0;
    }
}

bool FloodFill::spaceRight(){
    switch (m_d){
        case NORTH:
            return m_x+1 < MAZE_SIZE;
        case EAST:
            return m_y > 0;
        case SOUTH:
            return m_x > 0;
        case WEST:
            return m_y+1 < MAZE_SIZE;
    }
}

void FloodFill::explore(){

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
    std::stack<Cell*> unexplored;

    // The initial square is at (0, 0)
    unexplored.push(&m_cells[0][0]);

    // The explore method needs to explore all Cells, so we loop until unexplored is empty.
    // The other condition ensures that any requests, regardless of whether or not the
    // stack was empty at the time of the request (which could happen as we visit the
    // the last target, or in other weird cases), are honored and handled appropriately.
    while (!unexplored.empty() || (m_mouse->undoRequested() || m_mouse->resetRequested()) ) {

        if (m_mouse->resetRequested()) {
            m_mouse->resetPosition();
            m_mouse->resetColors(0, 0);
            m_mouse->resetHonored();
            return;
        }

        if (m_mouse->undoRequested()) {

            // We check to see if we've moved at least SHORT_TERM_MEM number of times. If so, then
            // we can go back that many steps to the state that existed right BEFORE those steps

            m_x = 0;
            m_y = 0;
            m_d = 0;
            m_mouse->resetPosition();

            // We haven't yet gotten to the checkpoint. If another undo request is made,
            // don't reset the checkpoint. Simply try to get back to the same checkpoint.
            m_checkpointReached = false;

            // We must call undoHonored() directly after we decide to honor the request
            // so that we may detect more undo requests during our attempt to continue
            // to the checkpoint the maze.
            m_mouse->undoHonored();

            // If an undo is requested within the first few steps, we can simply
            // re-explore the maze since we haven't made an real progress yet.
            // Doing things this way ensures that some corner cases are handled.
            if (m_history.getCheckpointCell() == &m_cells[0][0]) {
                return;
            }

            // Retrieve the old stack image.
            unexplored = m_history.getCheckpointStack();

            // Retrieve the path to get back to the current cell.
            std::stack<Cell*> path = m_history.getCheckpointPath();

            // Clear the modified cells.
            m_history.resetModifiedCells();

            // Moves to the checkpoint and checks for requests along the way.
            if (proceedToCheckpoint(path)) {
                if (m_mouse->resetRequested()) {
                    m_mouse->resetPosition();
                    m_mouse->resetColors(0, 0);
                    m_mouse->resetHonored();
                    return;
                }
                else {
                    // If another undo is requested while this undo is still being completed,
                    // then we should simply redo our attempts to get to the checkpoint cell
                    continue;
                }
            }

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
        Cell* target = NULL;

        // The first condition for looping, namely that all not all walls be
        // inspected, is vital to the functioning of the rest of the explore
        // algorithm. Once this loop exits, we can be assured that either we've
        // explored everything, or that the target has not yet been traversed
        // (but will be traversed).

        while (allWallsInspected && !unexplored.empty()){

            // First, pop the target cell off of the stack
            target = unexplored.top(); 
            unexplored.pop();
    
            // Next, check to see if all of the walls have been inspected.
            // If so, no need to explore that cell - get a new target
            for (int i = 0; i < 4; i++){
                allWallsInspected = allWallsInspected && target->getWallInspected(i);
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

            if (allWallsInspected){
                target->setExplored(true);
                flood(target->getX(), target->getY());
            }
        }

        // We check for !allWallsInspected here in case the last thing on the stack had
        // all of its walls inspected. In that case, we don't want to do any more
        // updates or flooding - we simply want to return to the beginning for victory.
        if (allWallsInspected) {
            break;
        }

        // If prev == NULL we're at the start, so no need to move
        if (target->getPrev() != NULL){

            // Once we've found a valid, non-origin target (i.e. one that has not
            // had all of its walls already inspected), we move to the proper
            // advancing position for that particular Cell. That is, we keep
            // backtracking until we're one away from that Cell, or until a 
            // request is made (at which point we immediately honor that request).

            while (target->getPrev() != &m_cells[m_x][m_y]
                   && !(m_mouse->undoRequested() || m_mouse->resetRequested()) ) { // TODO

                // If at any point the target is one cell away, we've no need to
                // actually retrace though all of the other prev Cells. Thus we
                // can simply break to reduce the amount of retracing that we do

                if (isOneCellAway(target)){
                    break;
                }

                // Similarly, if the staging Cell (i.e., the Cell that the mouse has to
                // travel to before it can move to the target Cell) is one Cell away, 
                // we've no need to completely retrace though all of the other prev Cells.
                // We can simply move to the staging cell and proceed.

                if (isOneCellAway(target->getPrev())){
                    moveOneCell(target->getPrev());
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
                Cell* prev = m_cells[m_x][m_y].getPrev();
                moveOneCell(prev);
            }

            // Once the mouse is in proper advancing position, advance. However, if
            // a request was made, we broke out of the while-loop and thus cannot
            // be guaranteed that we're one Cell away from the target. Thus we
            // only move if no requests were made.
            if (!(m_mouse->undoRequested() || m_mouse->resetRequested())) {
                moveOneCell(target);
            }
        }

        // Once we're at the cell, perform the appropriate updates for that cell,
        // including modifying the explore stack. In the case that a request was
        // made (and therefore cannot be guaranteed that we're at the target cell),
        // updating does not have any effect since we're going to be undo that last
        // update in our short term memory (so long as our short term memory > 0).
        doUpdatesForCurrentCell(&unexplored);
    }

    // Once the stack is empty (once we've explored every possible cell),
    // we assign a maximum distance value to unexplored cells and then return
    // to the starting location. Thus, at the end of this function, all distance
    // values will be 100% complete and correct
    
    for (int x = 0; x < MAZE_SIZE; x++){
        for (int y = 0; y < MAZE_SIZE; y++){
            if (!m_cells[x][y].getExplored()){
                // Any unreachable cells should have inf distance. Conveniently,
                // MAZE_SIZE*MAZE_SIZE is one greater than the maximum distance
                m_cells[x][y].setDistance(MAZE_SIZE*MAZE_SIZE);
            }
        }
    }

    // Mark the maze as fully explored. Note that we do this *prior* to attempting
    // to return to the beginning of the maze. The reason for this is that if we
    // make a mistake on our way back to the beginning, we can simply undo to
    // start from victory.
    m_explored = true;

    // Lastly, return to the starting location and face forward
    while (m_x != 0 || m_y != 0){
        Cell* prev = m_cells[m_x][m_y].getPrev();
        moveOneCell(prev);
    }
    while (m_d != 0){ 
        turnRight(); // Turning right is optimal since we'd never 
                     // approach the starting location from the left
    }
}

void FloodFill::moveOneCell(Cell* target){

    // Only move if the cell is actually one away
    if (isOneCellAway(target)){

        // Use local variables to reduce function calls
        int x = target->getX();
        int y = target->getY();
        
        // Directional logic
        int absDir = 0; // Assume that the direction is upward
        if (x < m_x){
            absDir = 3;        
        }
        else if (x > m_x){
            absDir = 1;        
        }
        else if (y < m_y){
            absDir = 2;        
        }

        // Turn the right direction
        if (absDir == m_d){
            // Do nothing - we're already facing the right way
        }
        else if (absDir == (m_d + 1)%4){
            turnRight();
        }
        else if (absDir == (m_d + 2)%4){
            turnRight();
            turnRight();
        }
        else if (absDir == (m_d + 3)%4){
            turnLeft();
        }

        //std::cout << "MoveOneCellForward" << std::endl;
        // Finally, move forward one space
        moveForward();
    }
    else{
        std::cout << "ERROR: Tried to move to cell (" << target->getX() << ","
        << target->getY() << ") but it's not one space away." << std::endl;
    }
}

void FloodFill::doUpdatesForCurrentCell(std::stack<Cell*>* unexplored) {

    // List of cellmod objects for all cell modifications performed at this step
    std::list<Cellmod> modifiedCells;

    // We need to keep track of the old values for the modified cell before we update it.
    appendModifiedCell(&modifiedCells, &m_cells[m_x][m_y]);

    // Once we're in the correct Cells, we can now examine the contents of the
    // target and update our walls and distances
    walls();
    flood(m_x, m_y);

    // Once we've examined the contents at the target, it is considered explored and traversed
    m_cells[m_x][m_y].setExplored(true);
    m_cells[m_x][m_y].setTraversed(true);

    // After, we find any unexplored neighbors.
    if (!m_mouse->wallLeft() && getLeftCell()->getPrev() == NULL){

        // We need to keep track of the old values for the modified cell before we update it.
        appendModifiedCell(&modifiedCells, getLeftCell());

        unexplored->push(getLeftCell());
        getLeftCell()->setPrev(&m_cells[m_x][m_y]);
    }
    if (!m_mouse->wallFront() && getFrontCell()->getPrev() == NULL){

        // We need to keep track of the old values for the modified cell before we update it.
        appendModifiedCell(&modifiedCells, getFrontCell());

        unexplored->push(getFrontCell());
        getFrontCell()->setPrev(&m_cells[m_x][m_y]);
    }
    if (!m_mouse->wallRight() && getRightCell()->getPrev() == NULL){

        // We need to keep track of the old values for the modified cell before we update it.
        appendModifiedCell(&modifiedCells, getRightCell());

        unexplored->push(getRightCell());
        getRightCell()->setPrev(&m_cells[m_x][m_y]);
    }

    // Update the History target stack and modified cells
    m_history.stackUpdate(*unexplored);
    m_history.modifiedCellsUpdate(modifiedCells);
}

bool FloodFill::isOneCellAway(Cell* target){

    // Use local variables to reduce function calls
    int x = target->getX();
    int y = target->getY();
    
    if ((m_x == x) && (m_y + 1 == y) && !m_cells[m_x][m_y].isWall(NORTH)){
        return true;
    }
    else if ((m_x == x) && (m_y - 1 == y) && !m_cells[m_x][m_y].isWall(SOUTH)){
        return true;
    }
    else if ((m_x + 1 == x) && (m_y == y) && !m_cells[m_x][m_y].isWall(EAST)){
        return true;
    }
    else if ((m_x - 1 == x) && (m_y == y) && !m_cells[m_x][m_y].isWall(WEST)){
        return true;
    }
    
    return false;
}

// Attempts to short-circuit the retracing by looking at neighbors of untraversed cells
// Returns true if successful, false if short-circuiting wasn't possible
bool FloodFill::tryUntraversed(Cell* target){

    int dir = -1; // 0: front, 1: right, 3:left (rear cannot be untraversed)
    int mostCount = -1; // Keeps track of highest number of short-circuited steps

    // First, find the direction with the greatest number of short circuited steps

    for (int i = 0; i < 3; i++) {

        Cell* neighbor = NULL;
        switch(i){
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
        if (isOneCellAway(neighbor) && !neighbor->getTraversed()) {

            // Find the cell that discovered the untraversed cell. Then check to see if *that*
            // cell is on the retracing path. If so, then we can short circuiting the retracing
            Cell* neighborPrev = neighbor->getPrev();

            // Sanity check
            if (neighborPrev != NULL) {

                // This pointer will trace back along the normal retracing path, checking to see
                // if neighborPrev is on the path (and more than one step away)
                Cell* runner = &m_cells[m_x][m_y];

                int counter = 0; // Counts how many steps that the mouse would have to retract
                                 // without short circuiting via the untraversed cell. If the
                                 // value of count ends up being more than one, we know that
                                 // it is *always* in our favor to retrace. Note that this also
                                 // helps us to choose the most profitable direction to move in
                                 // if multiple directions are untraversed.

                // This loop guarentees that we check for short circuiting up until the
                // prev Cell for the target. However, we need to check target->getPrev()
                // explicitly afterwards
                while (runner != target->getPrev()){

                    if (neighborPrev == runner) {
                        if (dir == -1 || counter < mostCount) {
                            dir = i;
                            mostCount = counter;
                        }
                        break; // Break out of the while-loop
                    }

                    counter++;
                    runner = runner->getPrev();
                }
                
                // At this point, we're guarenteed that runner == target->getPrev()
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
        Cell* untraversed = NULL; 
        switch(dir){
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
        moveOneCell(untraversed->getPrev());
        return true;
    }
    return false;
}

void FloodFill::basicFloodFill(){
    while (!inGoal(m_x, m_y)){
        walls();
        flood(m_x, m_y);
        moveTowardsGoal();
    }
}

bool FloodFill::proceedToCheckpoint(std::stack<Cell*> path) {

    while (!path.empty()) {

        // On each iteration, first check for requests
        if (m_mouse->resetRequested() || m_mouse->undoRequested()) {
            return true;
        }

        Cell* next = path.top();
        path.pop();

        // Since we popped the origin off of the path stack, we're guaranteed that we
        // won't try to moveOneCell() to the origin
        moveOneCell(next);
    }

    return false;
}

void FloodFill::appendModifiedCell(std::list<Cellmod>* modList, Cell* modCell) {

    Cellmod cm;
    cm.cell = modCell;
    cm.oldDist = modCell->getDistance();
    cm.oldPrev = modCell->getPrev();
    cm.oldExplored = modCell->getExplored(); // Note: I don't think this is necessary
    cm.oldTraversed = modCell->getTraversed(); // Note: I don't think this is necessary
    for (int i = 0; i < 4; i++) {
        cm.oldWalls[i] = modCell->isWall(i);
        cm.oldWallsInspected[i] = modCell->getWallInspected(i);
    }
    modList->push_back(cm);
}

bool FloodFill::checkRequestVictory() {

    // If a request has been made, perform appropriately. Since the maze has already
    // been completely solved, on reset we start over and on undo we simply retry
    // solving the maze as quickly as possible (without changing wall information)
    if (m_mouse->undoRequested() || m_mouse->resetRequested()) {
        m_mouse->resetPosition();
        if (m_mouse->resetRequested()) {
            // This return will cause victory() to exit, and thus
            // we reinitialize the maze and start our solve over
            m_mouse->resetColors(0, 0);
            m_mouse->resetHonored();
            return true;
        }
        else {
            m_x = 0; // Since we're repositioning the mouse but not re-intializing the
            m_y = 0; // maze, we have to explicitely reset the x, y, and d values
            m_d = 0;
            m_mouse->undoHonored();
        }
    }
    return false;
}
