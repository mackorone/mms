#include "FloodFill.h"

#include <iostream>
#include <stack>
#include <vector>

void FloodFill::solve(sim::MouseInterface* mouse){

    m_x = 0; // Initialize the x position of the mouse
    m_y = 0; // Initialize the y position of the mouse
    m_d = 0; // Initialize the direction position of the mouse
    m_steps = 0; // Initialize the mouse steps
    m_mouse = mouse; // Initialize the mouse pointer

    // Initialize the x and y positions of the cells
    for (int x = 0; x < MAZE_SIZE; x++){
        for (int y = 0; y < MAZE_SIZE; y++){
            m_cells[x][y].setX(x);
            m_cells[x][y].setY(y);
        }
    }

    // Initialize the Cells' walls and distances
    initialize();

    // Augmented Floodfill - Explore the maze to its entirety, returning to the start
    explore();
    std::cout << m_steps << std::endl; // Print the total number of steps

    // Compare the explore to the explore beta
    initialize();
    exploreBeta();
    std::cout << m_steps << std::endl; // Print the total number of steps

    // Loop forever, continue going to the beginning and solving
    while (true){

        std::stack<Cell*> optimalPath;

        // Solve the maze
        while (!inGoal()){
            optimalPath.push(&m_cells[m_x][m_y]);
            moveTowardsGoal();
        }

        // Return to start
        while (m_x != 0 || m_y != 0){
            Cell* prev = optimalPath.top();
            optimalPath.pop();
            moveOneCell(prev);
        }
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

void FloodFill::initialize(){

    // NOTE: Doesn't work for odd sized mazes

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
        }
    }

    // Set steps of the mouse to zero
    m_steps = 0;
}

void FloodFill::initializeDestinationTile(int x, int y){

    // Set this tile as the destination
    m_cells[x][y].setDistance(0);
    
    // Initialize the column
    for (int i = 0; i < y; i++){
        m_cells[x][i].setDistance(y-i);
    }
    for (int i = y+1; i < MAZE_SIZE; i++){
        m_cells[x][i].setDistance(i-y);
    }

    // Initialize the rows
    for (int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j < x; j++){
            m_cells[j][i].setDistance(x - j + m_cells[x][i].getDistance());
        }
        for (int j = x+1; j < MAZE_SIZE; j++){
            m_cells[j][i].setDistance(j - x + m_cells[x][i].getDistance());
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
    
    // *DO NOT* flood the maze if the mouse is in the goal - there is no
    // information that we can gain about the distance values in the goal,
    // and we can only corrupt the current distance values anyways
    if (!inGoal()){

        // Initialize distance values for surrounding cells
        int northDistance = MAZE_SIZE*MAZE_SIZE; // Max distance
        int eastDistance = MAZE_SIZE*MAZE_SIZE; // Max distance
        int southDistance = MAZE_SIZE*MAZE_SIZE; // Max distance
        int westDistance = MAZE_SIZE*MAZE_SIZE; // Max distance

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

    // Then, set the new Cell's explored flag to true
    m_cells[m_x][m_y].setExplored(true);

    // Actually move the mouse forward in the simulation
    m_mouse->moveForward();

    // Lastly, increment the number of steps
    m_steps++;
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

bool FloodFill::inGoal(){

    // The goal is defined to be the center of the maze 
    // This means that it's 4 squares of length if even, 1 if odd
    
    bool horizontal = (MAZE_SIZE - 1) / 2 == m_x;
    if (MAZE_SIZE % 2 == 0){
        horizontal = horizontal || (MAZE_SIZE) / 2 == m_x;
    }

    bool vertical = (MAZE_SIZE - 1) / 2 == m_y;
    if (MAZE_SIZE % 2 == 0){
        vertical = vertical || (MAZE_SIZE) / 2 == m_y;
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
    std::stack<Cell*> unexplored;

    // The initial square is at (0, 0)
    unexplored.push(&m_cells[0][0]);

    // Loop until all cells have been explored
    while (!unexplored.empty()){

        // Begin by assuming that all walls of the Cell that we intend to explore
        // have already been inspected. Thus, we assume that we have no need to
        // actually explore that Cell. We check the wallInspected values of each
        // of the walls of that Cell to confirm (or refute) our assumption. Once
        // we find a Cell for which not all of the walls have been inspected, we
        // proceed as normal.

        bool allWallsInspected = true;
        Cell* target = NULL; // Initialze the target cell

        while (allWallsInspected && !unexplored.empty()){

            // First, pop the target cell off of the stack
            target = unexplored.top(); 
            unexplored.pop();
    
            // Next, check to see if all of the walls have been inspected.
            // If so, no need to explore that cell - get a new target
            for (int i = 0; i < 4; i++){
                if (!target->getWallInspected(i)){
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
            if (allWallsInspected){
                target->setExplored(true);
                flood(target->getX(), target->getY());
            }
        }
    
        // Once we've found a valid target (i.e. one that has not had all of
        // its walls already inspected), we move to the proper advancing
        // position for that particular Cell. That is, we keep backtracking
        // until we're one away from that Cell.

        if (target->getPrev() != NULL){ // If prev == NULL we're at the start, so no need to move

            // While the mouse is not in the advancing position, trace back
            while (target->getPrev() != &m_cells[m_x][m_y]){

                // If at any point the target is one cell away, we've no need to
                // actually retrace though all of the other prev Cells. Thus we
                // can simply break to reduce the amount of retracing that we do

                if (isOneCellAway(target)){
                    break;
                }

                Cell* prev = m_cells[m_x][m_y].getPrev();
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
        m_cells[m_x][m_y].setExplored(true);
        
        // After, we find any unexplored neighbors
        if (!m_mouse->wallLeft() && getLeftCell()->getPrev() == NULL){
            unexplored.push(getLeftCell());
            getLeftCell()->setPrev(&m_cells[m_x][m_y]);
        }
        if (!m_mouse->wallFront() && getFrontCell()->getPrev() == NULL){
            unexplored.push(getFrontCell());
            getFrontCell()->setPrev(&m_cells[m_x][m_y]);
        }
        if (!m_mouse->wallRight() && getRightCell()->getPrev() == NULL){
            unexplored.push(getRightCell());
            getRightCell()->setPrev(&m_cells[m_x][m_y]);
        }
    }

    // Once the stack is empty (once we've explored every possible cell),
    // we assign a maximum distance value to unexplored cells and then return
    // to the starting location. Thus, at the end of this function, all distance
    // values will be 100% complete and correct
    
    for (int x = 0; x < MAZE_SIZE; x++){
        for (int y = 0; y < MAZE_SIZE; y++){
            if (!m_cells[x][y].getExplored()){
                // Any unreachable cells should have inf distance. Conveniently,
                // MAZE_SIZE*MAZE_SIZE is slightly greater than the maximum distance
                m_cells[x][y].setDistance(MAZE_SIZE*MAZE_SIZE);
            }
        }
    }

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

void FloodFill::exploreBeta(){
    
    /*
        The purpose of this function is to test any ideas for improving the
        explore method. Once improvements have been verified, they can be
        copied over to the regular explore method.
    */

    // Push unexplored nodes onto a stack
    std::stack<Cell*> unexplored;

    // The initial square is at (0, 0)
    unexplored.push(&m_cells[0][0]);

    // Loop until all cells have been explored
    while (!unexplored.empty()){

        // Begin by assuming that all walls of the Cell that we intend to explore
        // have already been inspected. Thus, we assume that we have no need to
        // actually explore that Cell. We check the wallInspected values of each
        // of the walls of that Cell to confirm (or refute) our assumption. Once
        // we find a Cell for which not all of the walls have been inspected, we
        // proceed as normal.

        bool allWallsInspected = true;
        Cell* target = NULL; // Initialze the target cell

        while (allWallsInspected && !unexplored.empty()){

            // First, pop the target cell off of the stack
            target = unexplored.top(); 
            unexplored.pop();
    
            // Next, check to see if all of the walls have been inspected.
            // If so, no need to explore that cell - get a new target
            for (int i = 0; i < 4; i++){
                if (!target->getWallInspected(i)){
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
            // accidentally assign it a maximum distance value, since we assign
            // these values based on whether or not a Cell was explored or not.
            // Note that there is no danger of setting an unreachable Cell as
            // explored, since an unreachable cell will be pushed onto the stack
            // in the first place
            if (allWallsInspected){
                target->setExplored(true);
                flood(target->getX(), target->getY());
            }
        }
    
        // Once we've found a valid target (i.e. one that has not had all of
        // its walls already inspected), we move to the proper advancing
        // position for that particular Cell. That is, we keep backtracking
        // until we're one away from that Cell.

        if (target->getPrev() != NULL){ // If prev == NULL we're at the start, so no need to move

            // While the mouse is not in the advancing position, trace back
            while (target->getPrev() != &m_cells[m_x][m_y]){

                // If at any point the target is one cell away, we've no need to
                // actually retrace though all of the other prev Cells. Thus we
                // can simply break to reduce the amount of retracing that we do

                if (isOneCellAway(target)){
                    break;
                }

                Cell* prev = m_cells[m_x][m_y].getPrev();
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
        m_cells[m_x][m_y].setExplored(true);
        
        // After, we find any unexplored neighbors
        if (!m_mouse->wallLeft() && getLeftCell()->getPrev() == NULL){
            unexplored.push(getLeftCell());
            getLeftCell()->setPrev(&m_cells[m_x][m_y]);
        }
        if (!m_mouse->wallFront() && getFrontCell()->getPrev() == NULL){
            unexplored.push(getFrontCell());
            getFrontCell()->setPrev(&m_cells[m_x][m_y]);
        }
        if (!m_mouse->wallRight() && getRightCell()->getPrev() == NULL){
            unexplored.push(getRightCell());
            getRightCell()->setPrev(&m_cells[m_x][m_y]);
        }
    }

    // Once the stack is empty (once we've explored every possible cell),
    // we assign a maximum distance value to unexplored cells and then return
    // to the starting location. Thus, at the end of this function, all distance
    // values will be 100% complete and correct
    
    for (int x = 0; x < MAZE_SIZE; x++){
        for (int y = 0; y < MAZE_SIZE; y++){
            if (!m_cells[x][y].getExplored()){
                // Any unreachable cells should have inf distance. Conveniently,
                // MAZE_SIZE*MAZE_SIZE is slightly greater than the maximum distance
                m_cells[x][y].setDistance(MAZE_SIZE*MAZE_SIZE);
            }
        }
    }

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

        // Finally, move forward one space
        moveForward();
    }
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

void FloodFill::basicFloodFill(){

    // Regular Floodfill Algorithm
    while (!inGoal()){
        walls();
        flood(m_x, m_y);
        moveTowardsGoal();
    }
}
