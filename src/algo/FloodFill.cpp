#include "FloodFill.h"

#include <algorithm> // TODO
#include <iostream> // TODO
using namespace std; // TODO


void FloodFill::solve(MouseInterface* mouse){
    m_mouse = mouse;
    floodSolve();
}

// ------------------------------- Flood Fill ------------------------------- //

/*
Author: Joseph Materski
Version: 2.0
Does not include steppers or harware specifics
Algorithm based on "An Efficient Algorithm for Robot Maze-Solving" by
Hongshe Dang, Jinguo Song, Qin Guo
*/
 
int floodArray[16][16]; //2 dimensions for the distance of each square
unsigned char wallArray[16][16]; // 2 dimensional array for walls
int xPos = 0; // x-coordinate of the bot
int yPos = 0; // y-coordinate of the bot
int relDir = 0; // Relative Direction of the bot

void FloodFill::floodSolve(){
    initializeGrid();
    print();
    while (true){
        updateWall(xPos, yPos); // Updates wall values based on walls surrounding mouse
        flood(xPos, yPos); // Floods the maze with new distance values based on walls
        decide(xPos, yPos); // Decides where to move next, and then moves
    }
}


// Prints the maze
void FloodFill::print(){
    std::cout << std::endl;
    for (int y = 15; y >= 0; y--){
        for (int x = 0; x < 16; x++){
            if ( floodArray[x][y] < 10){
                std::cout << " ";
            }
            std::cout << floodArray[x][y] << " ";
        }
        std::cout << std::endl;
    }
}

void FloodFill::initializeGrid(){

    // Initialize the distances
    int distance = 14;
    for (int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            floodArray[x][y] = distance; // Bottom-left
            floodArray[y][15-x] = distance; // Upper-left
            floodArray[15-x][15-y] = distance; // Upper-right
            floodArray[15-y][x] = distance; // Bottom right
            distance -= 1;
        }
        distance += 7;
    }

    // Initialize the walls
    for (int y = 0; y < 16; y++){
        for (int x = 0; x < 16; x++){
            if (y == 0){
                wallArray[x][y] |= 0x4;
            }
            if (y == 15){
                wallArray[x][y] |= 0x1;
            }
            if (x == 0){
                wallArray[x][y] |= 0x8;
            }
            if (x == 15){
                wallArray[x][y] |= 0x2;
            }
        }
    }
}

//updatedWall method
void FloodFill::updateWall(int x, int y){ // These methods check to see if there are walls to left, right, or front
    leftWall(x, y);
    rightWall(x, y);
    frontWall(x, y);
    print();
}

void FloodFill::leftWall(int x, int y){
    if(m_mouse->wallLeft()){
        //std::cout << "Wall left\n" << std::endl; // TODO
        switch(relDir){
            case 0:
                wallArray[xPos][yPos] |= 0x8;
                if (xPos > 0){
                    wallArray[xPos-1][yPos] |= 0x2;
                }
                break;
            case 1:
                wallArray[xPos][yPos] |= 0x1;
                if (yPos > 0){
                    wallArray[xPos][yPos+1] |= 0x4;
                }
                break;
            case 2:
                wallArray[xPos][yPos] |= 0x2;
                if (xPos < 15){
                    wallArray[xPos+1][yPos] |= 0x8;
                }
                break;
            case 3:
                wallArray[xPos][yPos] |= 0x4;
                if (y < 15){
                    wallArray[xPos][yPos-1] |= 0x1;
                }
                break;
        }
    }
}

void FloodFill::rightWall(int x, int y){
    if(m_mouse->wallRight()){
        //std::cout << "Wall right\n" << std::endl; // TODO
        switch(relDir){
            case 0:
                wallArray[xPos][yPos] |= 0x2;
                if (xPos < 15){
                    wallArray[xPos+1][yPos] |= 0x8;
                }
                break;
            case 1:
                wallArray[xPos][yPos] |= 0x4;
                if (y < 15){
                    wallArray[xPos][yPos-1] |= 0x1;
                }
                break;
            case 2:
                wallArray[xPos][yPos] |= 0x8;
                if (xPos > 0){
                    wallArray[xPos+1][yPos] |= 0x2;
                }
                break;
            case 3:
                wallArray[xPos][yPos] |= 0x1;
                if (y > 0){
                    wallArray[xPos][yPos+1] |= 0x4;
                }
                break;
        }   
    }
}

void FloodFill::frontWall(int x, int y){
    if(m_mouse->wallFront()){
        //std::cout << "Wall front\n" << std::endl; // TODO
        switch (relDir){
            case 0:
                wallArray[xPos][yPos] |= 0x1;
                if (y > 0){
                    wallArray[xPos][yPos+1] |= 0x4;
                }
                break;
            case 1:
                wallArray[xPos][yPos] |= 0x2;
                if (xPos < 15){
                    wallArray[xPos+1][yPos] |= 0x8;
                }
                break;
            case 2:
                wallArray[xPos][yPos] |= 0x4;
                if (y < 15){
                    wallArray[xPos][yPos-1] |= 0x1;
                }
                break;
            case 3:
                wallArray[xPos][yPos] |= 0x8;
                if (xPos > 0){
                    wallArray[xPos+1][yPos] |= 0x2;
                }
                break;
        }   
    }
}

void FloodFill::flood(int x, int y){

    //print();
    //cout << "X: " << x << "  Y: " << y << " " << wallArray[x][y] << endl;

    switch(wallArray[x][y]){

        case(0x0): //no walls, 0x0 = 0000

            //takes the minimum of the sorrounding boxes and checks if it is 1 more
            if (floodArray[x][y] != min(floodArray[x-1][y], min(floodArray[x+1][y], 
                min(floodArray[x][y-1], floodArray[x][y+1]))) + 1){

                //otherwise it will set it to 1 more and call a flood on surrounding boxes
                floodArray[x][y] == min(floodArray[x-1][y], min(floodArray[x+1][y],
                min(floodArray[x][y-1], floodArray[x][y+1]))) + 1;
                flood(x,y+1);
                flood(x-1,y);
                flood(x+1,y);
                flood(x,y-1);
            }
            break;


        case(0x1): //U, 0x1=0001

            //this time it doesnt check the one above because there is a wall there
            if (floodArray[x][y] != min(floodArray[x-1][y], 
                min(floodArray[x+1][y], floodArray[x][y-1])) + 1){

                //again not checking or calling the flood on the one above it
                floodArray[x][y] = min(floodArray[x-1][y], min(floodArray[x+1][y], floodArray[x][y-1])) + 1;
                flood(x-1,y);
                flood(x+1,y);
                flood(x,y-1);
            }
            break;


        case(0x2): //R

            if (floodArray[x][y] != min(floodArray[x-1][y], 
                min(floodArray[x][y-1], floodArray[x][y+1])) + 1){

                floodArray[x][y] = min(floodArray[x-1][y],min(floodArray[x][y-1],floodArray[x][y+1])) + 1;
                flood(x,y+1);
                flood(x-1,y);
                flood(x,y-1);
            }
            break;


        case(0x3): //UR

            if (floodArray[x][y] != min(floodArray[x-1][y],floodArray[x][y-1]) + 1){

                floodArray[x][y] = min(floodArray[x-1][y],floodArray[x][y-1]) + 1;
                flood(x-1,y);
                flood(x,y-1);
            }
            break;


        case(0x4): //D

            if (floodArray[x][y] != min(floodArray[x-1][y],
                min(floodArray[x+1][y], floodArray[x][y+1])) + 1){

                floodArray[x][y] = min(floodArray[x-1][y],min(floodArray[x+1][y],floodArray[x][y+1])) + 1;
                flood(x,y+1);
                flood(x-1,y);
                flood(x+1,y);
            }
            break;


        case(0x5): //UD

            if (floodArray[x][y] != min(floodArray[x-1][y],floodArray[x+1][y]) + 1){

                floodArray[x][y] = min(floodArray[x-1][y],floodArray[x+1][y]) + 1;
                flood(x-1,y);
                flood(x+1,y);
            }
            break;


        case(0x6): //RD

            if (floodArray[x][y] != min(floodArray[x-1][y],floodArray[x][y+1]) + 1){

                floodArray[x][y] = min(floodArray[x-1][y],floodArray[x][y+1]) + 1;
                flood(x,y+1);
                flood(x-1,y);
            }
            break;


        case(0x7): //URD

            if (floodArray[x][y] != floodArray[x-1][y] + 1){

                floodArray[x][y] = floodArray[x-1][y] + 1;
                flood(x-1,y);
            }
            break;


        case(0x8): //L

            if (floodArray[x][y] != min(floodArray[x+1][y],
                min(floodArray[x][y-1],floodArray[x][y+1])) + 1){

                floodArray[x][y] = min(floodArray[x+1][y],min(floodArray[x][y-1],floodArray[x][y+1])) + 1;
                flood(x,y+1);
                flood(x,y-1);
                flood(x+1,y);
            }
            break;


        case(0x9): //LU

            if (floodArray[x][y] != min(floodArray[x+1][y],floodArray[x][y-1]) + 1){

                floodArray[x][y] = min(floodArray[x+1][y],floodArray[x][y-1]) + 1;
                flood(x,y-1);
                flood(x+1,y);
            }
            break;


        case(0xA): //LR

            if (floodArray[x][y] != min(floodArray[x][y-1],floodArray[x][y+1]) + 1){

                floodArray[x][y] = min(floodArray[x][y-1],floodArray[x][y+1]) + 1;
                flood(x,y+1);
                flood(x,y-1);
            }
            break;


        case(0xB): //LUR

            if (floodArray[x][y] != floodArray[x][y-1] + 1){

                floodArray[x][y] = floodArray[x][y-1] + 1;
                flood(x,y-1);
            }
            break;


        case(0xC): //LD

            if (floodArray[x][y] != min(floodArray[x+1][y],floodArray[x][y+1]) + 1){

                floodArray[x][y] = min(floodArray[x+1][y],floodArray[x][y+1]) + 1;
                flood(x,y+1);
                flood(x+1,y);
            }
            break;


        case(0xD): //LUD

            if (floodArray[x][y] != floodArray[x+1][y] + 1){

                floodArray[x][y] = floodArray[x+1][y] + 1;
                flood(x+1,y);
            }
            break;


        case(0xE): //LRD

            if (floodArray[x][y] != floodArray[x][y+1] + 1){

                floodArray[x][y] = floodArray[x][y+1] + 1;
                flood(x,y+1);
            }
            break;

        // There is no case 0xF since the bot cannot be completely enclosed
    }
}

void FloodFill::decide(int x, int y){
    switch(wallArray[x][y]){
        case 0x0:
            findLowest(floodArray[x-1][y], floodArray[x][y+1], floodArray[x+1][y], floodArray[x][y-1]);
            break;
        case 0x1:
            findLowest(floodArray[x-1][y], 1000,floodArray[x+1][y], floodArray[x][y-1]);
            break;
        case 0x2:
            findLowest(floodArray[x-1][y], floodArray[x][y+1], 1000, floodArray[x][y-1]);
            break;
        case 0x3:
            findLowest(floodArray[x-1][y], 1000, 1000, floodArray[x][y-1]);
            break;
        case 0x4:
            findLowest(floodArray[x-1][y], floodArray[x][y+1], floodArray[x+1][y], 1000);
            break;
        case 0x5:
            findLowest(floodArray[x-1][y], 1000, floodArray[x+1][y], 1000);
            break;
        case 0x6:
            findLowest(floodArray[x-1][y], floodArray[x][y+1], 1000, 1000);
            break;
        case 0x7:
            findLowest(floodArray[x-1][y], 1000, 1000, 1000);
            break;
        case 0x8:
            findLowest(1000, floodArray[x][y+1], floodArray[x+1][y], floodArray[x][y-1]);
            break;
        case 0x9:
            findLowest(1000,1000,floodArray[x+1][y], floodArray[x][y-1]);
            break;
        case 0xA:
            findLowest(1000,floodArray[x][y+1],1000, floodArray[x][y-1]);
            break;
        case 0xB:
            findLowest(1000,1000,1000, floodArray[x][y-1]);
            break;
        case 0xC:
            findLowest(1000,floodArray[x][y+1],floodArray[x+1][y], 1000);
            break;
        case 0xD:
            findLowest(1000,1000,floodArray[x+1][y], 1000);
            break;
        case 0xE:
            findLowest(1000,floodArray[x][y+1],1000, 1000);
            break;
        case 0xF:
            //Serial.print("Damn!");
            break;
    }
}

void FloodFill::findLowest(int left, int up, int right, int down){
    int lowest;
    int lowDir;
    switch(relDir){
        case 0:
            lowest = up;
            lowDir = 0;
            if (right<lowest){ lowest=right; lowDir = 1;}
            if (down<lowest){ lowest=down; lowDir = 2; }
            if (left<lowest){ lowest=left; lowDir = 3; }
            switch(lowDir){
                case 0:
                    forward();
                    break;
                case 1:
                    turnRight();
                    break;
                case 2:
                    turnAround();
                    break;
                case 3:
                    turnLeft();
                    break;
            }
            break;

        case 1:
            lowest = right;
            lowDir = 1;
            if (up<lowest){ lowest=up; lowDir = 0;}
            if (down<lowest){ lowest=down; lowDir = 2; }
            if (left<lowest){ lowest=left; lowDir = 3; }
            switch(lowDir){
                case 0:
                    turnLeft();
                    break;
                case 1:
                    forward();
                    break;
                case 2:
                    turnRight();
                    break;
                case 3:
                    turnAround();
                    break;
            }
            break;
    
        case 2:
            lowest = down;
            lowDir = 2;
            if (up<lowest){ lowest=up; lowDir = 0; }
            if (right<lowest){ lowest=right; lowDir = 1;}
            if (left<lowest){ lowest=left; lowDir = 3; }
            switch(lowDir){
                case 0:
                    turnAround();
                    break;
                case 1:
                    turnLeft();
                    break;
                case 2:
                    forward();
                    break;
                case 3:
                    turnRight();
                    break;
            }
            break;

        case 3:
            lowest = left;
            lowDir = 3;
            if (up<lowest){ lowest=up; lowDir = 0; }
            if (right<lowest){ lowest=right; lowDir = 1;}
            if (down<lowest){ lowest=down; lowDir = 2; }
            switch(lowDir){
                case 0:
                    turnRight();
                    break;
                case 1:
                    turnAround();
                    break;
                case 2:
                    turnLeft();
                    break;
                case 3:
                    forward();
                    break;
            }
            break;
    }

    std::cout << "Lowest was :" << lowest << std::endl;
    std::cout << "Lowest direction was :" << lowDir << std::endl;
}

void FloodFill::forward(){
    switch(relDir){
        case 0:
            yPos++;
            break;
        case 1:
            xPos++;
            break;
        case 2:
            yPos--;
            break;
        case 3:
            xPos--;
            break;
    }

    m_mouse->moveForward();
}

void FloodFill::turnLeft(){
    relDir = (relDir + 3) % 4;
    m_mouse->turnLeft();
    forward();
}

void FloodFill::turnRight(){
    relDir = (relDir + 1) % 4;
    m_mouse->turnRight();
    forward();
}

void FloodFill::turnAround(){
    m_mouse->turnRight();
    m_mouse->turnRight();
    relDir = (relDir + 2) % 4;
    forward();
}
