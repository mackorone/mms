#include "FloodFill.h"

#include <iostream>

void FloodFill::solve(MouseInterface* mouse){
    m_x = 0; // Initialize the x
    m_y = 0; // Initialize the y
    m_d = 0; // Initialize the direction
    m_mouse = mouse; // Initialize the mouse pointer
    initialize(); // Initialize the cells
    while (!inGoal()){
        walls();
        flood(m_x, m_y);
        //printDistances();
        move();
    }
    // Return to start, find shortest path
    /*initializeDestinationTile(15, 9);
    printDistances();
    while (m_x != 15 || m_y != 9){
        walls();
        flood(m_x, m_y);
        printDistances();
        move();
    }*/
}

void FloodFill::printDistances(){
    std::cout << std::endl;
    for (int y = MAZE_SIZE-1; y >= 0; y--){
        for (int x = 0; x < MAZE_SIZE; x++){
            if (m_cells[x][y].distance() < 10){
                std::cout << " ";
            }
            std::cout << m_cells[x][y].distance() << " ";
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

    // Initialize the distances
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

    // Initialize the walls
    for (int y = 0; y < MAZE_SIZE; y++){
        for (int x = 0; x < MAZE_SIZE; x++){
            m_cells[x][y].setWall(SOUTH, y == 0);
            m_cells[x][y].setWall(NORTH, y == MAZE_SIZE-1);
            m_cells[x][y].setWall(WEST, x == 0);
            m_cells[x][y].setWall(EAST, x == MAZE_SIZE-1);
        }
    }
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
            m_cells[j][i].setDistance(x - j + m_cells[x][i].distance());
        }
        for (int j = x+1; j < MAZE_SIZE; j++){
            m_cells[j][i].setDistance(j - x + m_cells[x][i].distance());
        }
    }
}

void FloodFill::walls(){

    // Sets the walls for the current cell
    m_cells[m_x][m_y].setWall(m_d, m_mouse->wallFront());
    m_cells[m_x][m_y].setWall((m_d+1)%4, m_mouse->wallRight());
    m_cells[m_x][m_y].setWall((m_d+3)%4, m_mouse->wallLeft());

    // Sets the walls for the surrounding cells
    if (spaceFront(m_x, m_y, m_d)){
        getFrontCell(m_x, m_y, m_d)->setWall((m_d+2)%4, m_mouse->wallFront());
    }
    if (spaceLeft(m_x, m_y, m_d)){
        getLeftCell(m_x, m_y, m_d)->setWall((m_d+1)%4, m_mouse->wallLeft());
    }
    if (spaceRight(m_x, m_y, m_d)){
        getRightCell(m_x, m_y, m_d)->setWall((m_d+3)%4, m_mouse->wallRight());
    }
}

void FloodFill::flood(int x, int y){
    
    // Initialize distance values for surrounding cells
    int northDistance = MAZE_SIZE*MAZE_SIZE; // Max distance
    int eastDistance = MAZE_SIZE*MAZE_SIZE; // Max distance
    int southDistance = MAZE_SIZE*MAZE_SIZE; // Max distance
    int westDistance = MAZE_SIZE*MAZE_SIZE; // Max distance

    // Obtain actual values if possible
    if (!m_cells[x][y].isWall(NORTH)){
        northDistance = m_cells[x][y+1].distance();
    }
    if (!m_cells[x][y].isWall(EAST)){
        eastDistance = m_cells[x+1][y].distance();
    }
    if (!m_cells[x][y].isWall(SOUTH)){
        southDistance = m_cells[x][y-1].distance();
    }
    if (!m_cells[x][y].isWall(WEST)){
        westDistance = m_cells[x-1][y].distance();
    }

    // Check to see if the distance value is the min plus one
    if (m_cells[x][y].distance() != min(northDistance, eastDistance, southDistance, westDistance) + 1){

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


void FloodFill::move(){

    int minDistance = m_cells[m_x][m_y].distance(); // Initialize distance
    int relDirection = 0; // Initialize direction

    if (!m_cells[m_x][m_y].isWall((m_d+3)%4)){ 
        if (getLeftCell(m_x, m_y, m_d)->distance() <= minDistance){
            minDistance = getLeftCell(m_x, m_y, m_d)->distance();
            relDirection = 3;
        }
    }
    if (!m_cells[m_x][m_y].isWall((m_d+2)%4)){ 
        if (getRearCell(m_x, m_y, m_d)->distance() <= minDistance){
            minDistance = getRearCell(m_x, m_y, m_d)->distance();
            relDirection = 2;
        }
    }
    if (!m_cells[m_x][m_y].isWall((m_d+1)%4)){ 
        if (getRightCell(m_x, m_y, m_d)->distance() <= minDistance){
            minDistance = getRightCell(m_x, m_y, m_d)->distance();
            relDirection = 1;
        }
    }
    if (!m_cells[m_x][m_y].isWall(m_d)){ 
        if (getFrontCell(m_x, m_y, m_d)->distance() <= minDistance){
            minDistance = getFrontCell(m_x, m_y, m_d)->distance();
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

void FloodFill::turnRight(){
    m_d = (m_d + 1) % 4;
    m_mouse->turnRight();
}

void FloodFill::turnLeft(){
    m_d = (m_d + 3) % 4;
    m_mouse->turnLeft();
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

Cell* FloodFill::getFrontCell(int x, int y, int d){
    switch (d){
        case NORTH:
            return &m_cells[x][y+1];
        case EAST:
            return &m_cells[x+1][y];
        case SOUTH:
            return &m_cells[x][y-1];
        case WEST:
            return &m_cells[x-1][y];
    }
}

Cell* FloodFill::getLeftCell(int x, int y, int d){
    switch (d){
        case NORTH:
            return &m_cells[x-1][y];
        case EAST:
            return &m_cells[x][y+1];
        case SOUTH:
            return &m_cells[x+1][y];
        case WEST:
            return &m_cells[x][y-1];
    }
}

Cell* FloodFill::getRightCell(int x, int y, int d){
    switch (d){
        case NORTH:
            return &m_cells[x+1][y];
        case EAST:
            return &m_cells[x][y-1];
        case SOUTH:
            return &m_cells[x-1][y];
        case WEST:
            return &m_cells[x][y+1];
    }
}

Cell* FloodFill::getRearCell(int x, int y, int d){
    switch (d){
        case NORTH:
            return &m_cells[x][y-1];
        case EAST:
            return &m_cells[x-1][y];
        case SOUTH:
            return &m_cells[x][y+1];
        case WEST:
            return &m_cells[x+1][y];
    }
}

bool FloodFill::spaceFront(int x, int y, int d){
    switch (d){
        case NORTH:
            return y+1 < MAZE_SIZE;
        case EAST:
            return x+1 < MAZE_SIZE;
        case SOUTH:
            return y > 0;
        case WEST:
            return x > 0;
    }
}

bool FloodFill::spaceLeft(int x, int y, int d){
    switch (d){
        case NORTH:
            return x > 0;
        case EAST:
            return y+1 < MAZE_SIZE;
        case SOUTH:
            return x+1 < MAZE_SIZE;
        case WEST:
            return y > 0;
    }
}

bool FloodFill::spaceRight(int x, int y, int d){
    switch (d){
        case NORTH:
            return x+1 < MAZE_SIZE;
        case EAST:
            return y > 0;
        case SOUTH:
            return x > 0;
        case WEST:
            return y+1 < MAZE_SIZE;
    }
}
