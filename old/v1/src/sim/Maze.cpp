#include "Maze.h"

#include <cstdlib>
#include <math.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <time.h>
#include <queue>
#include <vector>
#include <stack>

#include "Sleep.h"
#include "../sim/Constants.h"
#include "../sim/Parameters.h"
#include "../sim/Tile.h"

namespace sim {

Maze::Maze(int width, int height, std::string mazeFileDirPath, std::string mazeFile) {

    // Misc errand - seed the random number generator
    srand(time(NULL));
    
    // Initialize the tile positions
    for (int x = 0; x < width; x += 1){
        std::vector<Tile> col;
        for (int y = 0; y < height; y += 1){
            Tile tile;
            tile.setPos(x, y);
            col.push_back(tile);
        }
        m_maze.push_back(col);
    }
    
    // Initialize the tile wall values and tile neighbors by either loading
    // an existing maze file or randomly generating a valid maze
    if (mazeFile == "") {

        std::cout << "No maze file provided. Generating random maze..." << std::endl;

        tom_random();
        while (!(solveShortestPath().at(0) && solveShortestPath().at(1))) {
            tom_random();
        }

        // Optional - can be used to generate more maze files
        saveMaze(mazeFileDirPath += "/auto_generated_maze.maz");
    }
    else {

        // Complete path to the given mazefile
        std::string path = mazeFileDirPath += mazeFile;

        // Check to see if the file exists
        std::fstream file(path.c_str());

        if (file){

            // Load the maze given by mazefile
            loadMaze(path);

            // Ensures that the maze is solvable
            if (!solveShortestPath().at(0)){
                std::cout << "Unsolvable Maze detected. Generating solvable maze..." << std::endl;
                while (!(solveShortestPath().at(0) && solveShortestPath().at(1))) {
                    tom_random();
                }
            }
        }
        else{

            // If the file doesn't exist, generate a random maze file
            std::cout << "File \"" << path << "\" not found. Generating random maze..." << std::endl;
            tom_random();
            while (!(solveShortestPath().at(0) && solveShortestPath().at(1))){
                tom_random();
            }
        }
    }
    
    // Increment the passes for the starting position
    m_maze.at(0).at(0).incrementPasses();
}

Maze::~Maze()
{ }

int Maze::getWidth(){
    return m_maze.size();
}

int Maze::getHeight(){
    if (m_maze.size() > 0){
        return m_maze.at(0).size();
    }
    return 0;
}

Tile* Maze::getTile(int x, int y){
    return &m_maze.at(x).at(y);
}

void Maze::resetColors(int curX, int curY){
    for (int x = 0; x < getWidth(); x += 1){
        for (int y = 0; y < getHeight(); y += 1){
            getTile(x, y)->resetPasses();
        }
    }
    getTile(curX, curY)->incrementPasses();
}

void Maze::randomize(){
    
    // Declare width and height locally to reduce function calls
    int width = getWidth();
    int height = getHeight();

    // Percentage chance any one wall will exist
    float wallProb = 0.35;

    // Randomly generate all of the walls
    for (int x = 0; x < width; x += 1){
        for (int y = 0; y < height; y += 1){
            for (int k = 0; k < 4; k += 1){
                bool wallExists = (rand() <= (RAND_MAX * wallProb));
                switch (k){
                    case NORTH:
                        if (y + 1 < height){
                            getTile(x, y+1)->setWall(SOUTH, wallExists);
                            getTile(x, y)->setWall(NORTH, wallExists);
                        }
                        else {
                            getTile(x, y)->setWall(NORTH, true);
                        }
                        break;
                    case EAST:
                        if (x + 1 < width){
                            getTile(x+1, y)->setWall(WEST, wallExists);
                            getTile(x, y)->setWall(EAST, wallExists);
                        }
                        else {
                            getTile(x, y)->setWall(EAST, true);
                        }
                        break;
                    case SOUTH:
                        if (y > 0){
                            getTile(x, y-1)->setWall(NORTH, wallExists);
                            getTile(x, y)->setWall(SOUTH, wallExists);
                        }
                        else {
                            getTile(x, y)->setWall(SOUTH, true);
                        }
                        break;
                    case WEST:
                        if (x > 0){
                            getTile(x-1, y)->setWall(EAST, wallExists);
                            getTile(x, y)->setWall(WEST, wallExists);
                        }
                        else {
                            getTile(x, y)->setWall(WEST, true);
                        }
                        break;
                }
            }
        }
    }

    // Make sure every peg has at least one wall
    for (int x = 0; x < width - 1; x += 1) {
        for (int y = 0; y < height - 1; y += 1) {
            if (!getTile(x, y)->isWall(NORTH)
             && !getTile(x, y)->isWall(EAST)
             && !getTile(x + 1, y + 1)->isWall(WEST)
             && !getTile(x + 1, y + 1)->isWall(SOUTH)) {
                switch (rand() / (1 + RAND_MAX/4)) {
                    case NORTH:
                        getTile(x + 1, y + 1)->setWall(WEST, true);
                        getTile(x, y + 1)->setWall(EAST, true);
                        break;
                    case EAST:
                        getTile(x + 1, y + 1)->setWall(SOUTH, true);
                        getTile(x + 1, y)->setWall(NORTH, true);
                        break;
                    case SOUTH:
                        getTile(x,y)->setWall(EAST, true);
                        getTile(x + 1,y)->setWall(WEST, true);
                        break;
                    case WEST:
                        getTile(x, y)->setWall(NORTH, true);
                        getTile(x, y + 1)->setWall(SOUTH, true);
                        break;
                }
            }
        }
    }

    // Ensures that the middle is hallowed out
    if (width % 2 == 0){
        if (height % 2 == 0){
            getTile(width/2-1, height/2-1)->setWall(NORTH, false);
            getTile(width/2-1, height/2)->setWall(SOUTH, false);
            getTile(width/2, height/2-1)->setWall(NORTH, false);
            getTile(width/2, height/2)->setWall(SOUTH, false);
            getTile(width/2-1, height/2-1)->setWall(EAST, false);
            getTile(width/2, height/2-1)->setWall(WEST, false);
        }
        getTile(width/2-1, height/2)->setWall(EAST, false);
        getTile(width/2, height/2)->setWall(WEST, false);
            
    }
    if (height % 2 == 0){
        getTile(width/2, height/2-1)->setWall(NORTH, false);
        getTile(width/2, height/2)->setWall(SOUTH, false);
    }

    // Once the walls are assigned, we can assign neighbors
    assignNeighbors();
}

void Maze::tom_random() {
    
    // Declare width and height locally to reduce function calls
    int width = getWidth();
    int height = getHeight();

    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {
            for (int i = 0; i < 4; i += 1){ 
                // Set all walls to true as algo will break down walls
                getTile(x, y)->setWall(i, true);
            }
        }
    }

    if (width % 2 == 0){   // mark middle as explored so dps avoids it
                           // mark distance as 120 (~median) so we can exclude center
                           // when looking for a wall to break down
        if (height % 2 == 0){
            getTile(width / 2, height / 2)->setExplored(true);
            getTile(width / 2, height / 2)->setDistance(120);

            getTile(width / 2 - 1, height / 2)->setExplored(true);
            getTile(width / 2 - 1, height / 2)->setDistance(120);

            getTile(width / 2, height / 2 - 1)->setExplored(true);
            getTile(width / 2, height / 2 - 1)->setDistance(120);

            getTile(width / 2 - 1, height / 2 - 1)->setExplored(true);
            getTile(width / 2 - 1, height / 2 - 1)->setDistance(120);
        }
        getTile(width / 2 - 1, height / 2)->setExplored(true);
        getTile(width / 2 - 1, height / 2)->setDistance(120);

        getTile(width / 2, height / 2)->setExplored(true);
        getTile(width / 2, height / 2)->setDistance(120);

    }
    if (height % 2 == 0){
        getTile(width / 2, height / 2 - 1)->setExplored(true);
        getTile(width / 2, height / 2 - 1)->setDistance(120);

        getTile(width / 2, height / 2)->setExplored(true);
        getTile(width / 2, height / 2)->setDistance(120);
    }

    // We'll use a stack to do a DFS
    std::stack<Tile*> toExplore;

    getTile(0, 0)->setExplored(true); // Put the first cell on stack
    getTile(0, 0)->setDistance(0); // Cell 0
    Tile* t = getTile(0,0);
    toExplore.push(t);

    int exploreDIR = -1; //last direction the mouse moved. -1 if DNE

    float moveConst = 0; // the chance the algorithm will continue same direction of tavel
                         // if zero the maze will always turn. if 1 maze will be longest possible
                        // Currently proprtional to distance from center <see below>

    int dirThreshold = RAND_MAX * moveConst; // the threshold which the random number will be 
                                                      // compared too
    
    // Continue to DFS until we've explored every Tile
    while (!toExplore.empty()){
        // Grab and unpack the top thing from the stack
        Tile* current = toExplore.top();

        int x_pos = current->getX();
        int y_pos = current->getY();

        float xDelta = (abs(x_pos - width / 2.0) * 2.0) / (width-1); //ratios of how far you are from the center
        float yDelta = (abs(y_pos - height / 2.0) * 2.0) / (height-1);

        float avgDelta = std::max(xDelta, yDelta) * 0.85; // * 0.85 gives good compromise betwwen straight and turned

        moveConst = 1 * avgDelta; // the chance the algorithm will continue same direction of tavel is proportional
                                  // to the max distance of the mouse to the center on any axis.

        dirThreshold = RAND_MAX * moveConst;

        // Keep track of the next possible movements N = 0, E = 1, S = 2, W =3
        bool choices[4] = {0,0,0,0};

        // If cell to the North is valid and unexplored
        if ((y_pos + 1 < MAZE_WIDTH) && !getTile(x_pos, y_pos + 1)->getExplored()) {
            choices[NORTH] = true;
        }
        // If cell to the East is valid and unexplored
        if ((x_pos + 1 < MAZE_WIDTH) && !getTile(x_pos + 1, y_pos)->getExplored()) {
            choices[EAST] = true;
        }
        // If cell to the South is valid and unexplored
        if ((y_pos - 1 >= 0) && !getTile(x_pos, y_pos - 1)->getExplored()) {
            choices[SOUTH] = true;
        }
        // If cell to the West is valid and unexplored
        if ((x_pos - 1 >= 0) && !getTile(x_pos - 1, y_pos)->getExplored()) {
            choices[WEST] = true;
        }


        int possible = choices[0] + choices[1] + choices[2] + choices[3]; // Sum of possible moves

        // If the current cell has no more paths forward, we're done
        if (possible == 0) {
            toExplore.pop(); // Go Back
            if (exploreDIR != -1){ // We just reached the end of a path. lets break a wall with probability P
                if (rand() <= RAND_MAX * .75){ // HULK SMASH the wall that will connect two most disjoint
                    
                    int currentCellDist = getTile(x_pos, y_pos)->getDistance();
                    int biggestDifference = 0;
                    int cellToBreak = 0; // NORTH, SOUTH, EAST, WEST

                    // If cell to the North is valid and explored
                    if ((y_pos + 1 < MAZE_WIDTH) && getTile(x_pos, y_pos + 1)->getExplored()) {
                        if (abs(getTile(x_pos, y_pos + 1)->getDistance() - currentCellDist) > biggestDifference){
                            biggestDifference = abs(getTile(x_pos, y_pos + 1)->getDistance() - currentCellDist);
                            cellToBreak = NORTH;
                        }
                    }
                    // If cell to the East is valid and explored
                    if ((x_pos + 1 < MAZE_WIDTH) && getTile(x_pos + 1, y_pos)->getExplored()) {
                        if (abs(getTile(x_pos + 1, y_pos)->getDistance() - currentCellDist) > biggestDifference){
                            biggestDifference = abs(getTile(x_pos + 1, y_pos)->getDistance());
                            cellToBreak = EAST;
                        }
                    }
                    // If cell to the South is valid and explored
                    if ((y_pos - 1 >= 0) && getTile(x_pos, y_pos - 1)->getExplored()) {
                        if (abs(getTile(x_pos, y_pos - 1)->getDistance() - currentCellDist) > biggestDifference){
                            biggestDifference = abs(getTile(x_pos, y_pos - 1)->getDistance());
                            cellToBreak = SOUTH;
                        }
                    }
                    // If cell to the West is valid and explored
                    if ((x_pos - 1 >= 0) && getTile(x_pos - 1, y_pos)->getExplored()) {
                        if (abs(getTile(x_pos - 1, y_pos)->getDistance() - currentCellDist) > biggestDifference){
                            biggestDifference = abs(getTile(x_pos - 1, y_pos)->getDistance());
                            cellToBreak = WEST;
                        }
                    }

                    if (biggestDifference > 0){ // ... figure out why '>5' still generates solo pegs
                        switch (cellToBreak){ // Break wall across greates gradiant
                        case NORTH:
                            getTile(x_pos, y_pos)->setWall(NORTH, false);
                            getTile(x_pos, y_pos + 1)->setWall(SOUTH, false);
                            break;
                        case SOUTH:
                            getTile(x_pos, y_pos)->setWall(SOUTH, false);
                            getTile(x_pos, y_pos - 1)->setWall(NORTH, false);
                            break;
                        case EAST:
                            getTile(x_pos, y_pos)->setWall(EAST, false);
                            getTile(x_pos + 1, y_pos)->setWall(WEST, false);
                            break;
                        case WEST:
                            getTile(x_pos, y_pos)->setWall(WEST, false);
                            getTile(x_pos - 1, y_pos)->setWall(EAST, false);
                            break;
                        }
                    }
                }

            }
            exploreDIR = -1; // Once we start backtracing there is no last move
            continue;
        }
        
        int direction;  // final direction holder
        int rand_dir;   // random variable holder


        //if previous move exists, and repeating it will yeild a valid move AND its not the only valid move

        if (exploreDIR != -1 && choices[exploreDIR] != 0 && possible != 1)
        {
            if (rand() <= dirThreshold){  // if random variable is below threshold move in last direction
                direction = exploreDIR;
            }
            else {
                rand_dir = rand() / (1 + RAND_MAX / 4.0); // otherwise move in any viable direction
                while (choices[rand_dir] == 0 || rand_dir == exploreDIR) { // but the last moved direction
                    rand_dir = rand() / (1 + RAND_MAX / 4.0);
                }
                direction = rand_dir;
            }
        }
        else { // otherwise just move in a random viable direction
            rand_dir = rand() / (1 + RAND_MAX / 4.0);
            while (choices[rand_dir] == 0) {
                rand_dir = rand() / (1 + RAND_MAX / 4.0);
            }
            direction = rand_dir;
        }


        // Set the tile in the direction we move to explored and attach the number
        // Break down the wall between the current cell and that cell
        // And push it unto the stack

        switch (direction){
            case NORTH:
                getTile(x_pos, y_pos)->setWall(NORTH, false);
                getTile(x_pos, y_pos + 1)->setWall(SOUTH, false);
                getTile(x_pos, y_pos + 1)->setExplored(true);
                getTile(x_pos, y_pos + 1)->setDistance(toExplore.size()); // Size of stack is distance from center
                toExplore.push(getTile(x_pos, y_pos + 1)); // Push next cell unto stack
                exploreDIR = NORTH;
                break;
            case EAST:
                getTile(x_pos, y_pos)->setWall(EAST, false);
                getTile(x_pos + 1, y_pos)->setWall(WEST, false);
                getTile(x_pos + 1, y_pos)->setExplored(true);
                getTile(x_pos + 1, y_pos)->setDistance(toExplore.size());
                toExplore.push(getTile(x_pos + 1, y_pos));
                exploreDIR = EAST;
                break;
            case SOUTH:
                getTile(x_pos, y_pos)->setWall(SOUTH, false);
                getTile(x_pos, y_pos - 1)->setWall(NORTH, false);
                getTile(x_pos, y_pos - 1)->setExplored(true);
                getTile(x_pos, y_pos - 1)->setDistance(toExplore.size());
                toExplore.push(getTile(x_pos, y_pos - 1));
                exploreDIR = SOUTH;
                break;
            case WEST:
                getTile(x_pos, y_pos)->setWall(WEST, false);
                getTile(x_pos - 1, y_pos)->setWall(EAST, false);
                getTile(x_pos - 1, y_pos)->setExplored(true);
                getTile(x_pos - 1, y_pos)->setDistance(toExplore.size());
                toExplore.push(getTile(x_pos - 1, y_pos));
                exploreDIR = WEST;
                break;
            }
    }

    //Break one wall with the biggest gradiant across it down

    int biggestDifference = 0;
    int x_pos = 0; // x and y position at which the biggest diference occured
    int y_pos = 0;

    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {

            int currentCellDist = getTile(x, y)->getDistance();
            int prospectiveDist = 0;

            // If cell to the North is valid
            if ((y + 1 < MAZE_WIDTH)) {
                prospectiveDist = getTile(x, y + 1)->getDistance();
                //Check the gradient and see if it is the biggest one so far
                if (abs(prospectiveDist - currentCellDist) > biggestDifference){
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x; //Record x,y position to return later
                    y_pos = y;
                }
            }
            // If cell to the East is valid
            if ((x + 1 < MAZE_WIDTH)) {
                prospectiveDist = getTile(x + 1, y)->getDistance();
                if (abs(prospectiveDist - currentCellDist) > biggestDifference){
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x;
                    y_pos = y;
                }
            }
            // If cell to the South is valid
            if ((y - 1 >= 0)) {
                prospectiveDist = getTile(x, y - 1)->getDistance();
                if (abs(prospectiveDist - currentCellDist) > biggestDifference){
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x;
                    y_pos = y;
                }
            }
            // If cell to the West is valid
            if ((x - 1 >= 0)) {
                prospectiveDist = getTile(x - 1, y)->getDistance();
                if (abs(prospectiveDist - currentCellDist) > biggestDifference){
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x;
                    y_pos = y;
                }
            }
        }
    }
    
    int currentCellDist = getTile(x_pos, y_pos)->getDistance();
    
    // Break down wall of cell which is between the gradient of size 'biggestDifference'

    if ((y_pos + 1 < MAZE_WIDTH) && abs(getTile(x_pos, y_pos + 1)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference) {
        getTile(x_pos, y_pos)->setWall(NORTH, false);
        getTile(x_pos, y_pos + 1)->setWall(SOUTH, false);
    }
    if ((x_pos + 1 < MAZE_WIDTH) && abs(getTile(x_pos + 1, y_pos)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference) {
        getTile(x_pos, y_pos)->setWall(EAST, false);
        getTile(x_pos + 1, y_pos)->setWall(WEST, false);
    }
    if ((y_pos - 1 >= 0) && abs(getTile(x_pos, y_pos - 1)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference) {
        getTile(x_pos, y_pos)->setWall(SOUTH, false);
        getTile(x_pos, y_pos - 1)->setWall(NORTH, false);
    }
    if ((x_pos - 1 >= 0) && abs(getTile(x_pos - 1, y_pos)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference) {
        getTile(x_pos, y_pos)->setWall(WEST, false);
        getTile(x_pos - 1, y_pos)->setWall(EAST, false);
    }

    // Set each tile as unexplored and distance 255 by, the Mouse 
    // (note that we just "borrowed" these fields for the maze genereration.
    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {
            getTile(x, y)->setDistance(255);
            getTile(x, y)->setExplored(false);
        }
    }

    // Make sure every peg has at least one wall
    for (int x = 0; x < width - 1; x += 1) {
        for (int y = 0; y < height - 1; y += 1) {
            if (!getTile(x, y)->isWall(NORTH)
                && !getTile(x, y)->isWall(EAST)
                && !getTile(x + 1, y + 1)->isWall(WEST)
                && !getTile(x + 1, y + 1)->isWall(SOUTH)) {
                switch (rand() / (1 + RAND_MAX / 4)) {
                case NORTH:
                    getTile(x + 1, y + 1)->setWall(WEST, true);
                    getTile(x, y + 1)->setWall(EAST, true);
                    break;
                case EAST:
                    getTile(x + 1, y + 1)->setWall(SOUTH, true);
                    getTile(x + 1, y)->setWall(NORTH, true);
                    break;
                case SOUTH:
                    getTile(x, y)->setWall(EAST, true);
                    getTile(x + 1, y)->setWall(WEST, true);
                    break;
                case WEST:
                    getTile(x, y)->setWall(NORTH, true);
                    getTile(x, y + 1)->setWall(SOUTH, true);
                    break;
                }
            }
        }
    }

    // Ensures that the middle is hallowed out
    if (width % 2 == 0){
        if (height % 2 == 0){
            getTile(width/2-1, height/2-1)->setWall(NORTH, false);
            getTile(width/2-1, height/2)->setWall(SOUTH, false);
            getTile(width/2, height/2-1)->setWall(NORTH, false);
            getTile(width/2, height/2)->setWall(SOUTH, false);
            getTile(width/2-1, height/2-1)->setWall(EAST, false);
            getTile(width/2, height/2-1)->setWall(WEST, false);
        }
        getTile(width/2-1, height/2)->setWall(EAST, false);
        getTile(width/2, height/2)->setWall(WEST, false);
            
    }
    if (height % 2 == 0){
        getTile(width/2, height/2-1)->setWall(NORTH, false);
        getTile(width/2, height/2)->setWall(SOUTH, false);
    }

    // Break a random wall leading to center group and make sure other walls are present
    if (width % 2 == 0 && height % 2 != 0){ 

        getTile(width / 2 - 1, height / 2)->setWall(WEST, true);
        getTile(width / 2 - 2, height / 2)->setWall(EAST, true);

        getTile(width / 2 - 1, height / 2)->setWall(NORTH, true);
        getTile(width / 2 - 1, height / 2 + 1)->setWall(SOUTH, true);

        getTile(width / 2 - 1, height / 2)->setWall(SOUTH, true);
        getTile(width / 2 - 1, height / 2 - 1)->setWall(NORTH, true);

        getTile(width / 2, height / 2)->setWall(EAST, true);
        getTile(width / 2 + 1, height / 2)->setWall(WEST, true);

        getTile(width / 2, height / 2)->setWall(NORTH, true);
        getTile(width / 2, height / 2 + 1)->setWall(SOUTH, true);

        getTile(width / 2, height / 2)->setWall(SOUTH, true);
        getTile(width / 2, height / 2 - 1)->setWall(SOUTH, true);

        switch (rand() / (1 + RAND_MAX / 6)){
            case 0:
                getTile(width / 2 - 1, height / 2)->setWall(WEST, false);
                getTile(width / 2 - 2, height / 2)->setWall(EAST, false);
                break;
            case 1:
                getTile(width / 2 - 1, height / 2)->setWall(NORTH, false);
                getTile(width / 2 - 1, height / 2 + 1)->setWall(SOUTH, false);
                break;
            case 2:
                getTile(width / 2 - 1, height / 2)->setWall(SOUTH, false);
                getTile(width / 2 - 1, height / 2 - 1)->setWall(NORTH, false);
                break;
            case 3:
                getTile(width / 2, height / 2)->setWall(EAST, false);
                getTile(width / 2 + 1, height / 2)->setWall(WEST, false);
                break;
            case 4:
                getTile(width / 2, height / 2)->setWall(NORTH, false);
                getTile(width / 2, height / 2 + 1)->setWall(SOUTH, false);
                break;
            case 5:
                getTile(width / 2, height / 2)->setWall(SOUTH, false);
                getTile(width / 2, height / 2 - 1)->setWall(SOUTH, false);
                break;
        }
    }

    // Break a random wall leading to center group
    if (height % 2 == 0 && width % 2 != 0){

        getTile(width / 2, height / 2 - 1)->setWall(SOUTH, true);
        getTile(width / 2, height / 2 - 2)->setWall(NORTH, true);

        getTile(width / 2, height / 2 - 1)->setWall(EAST, true);
        getTile(width / 2 + 1, height / 2 - 1)->setWall(WEST, true);

        getTile(width / 2, height / 2 - 1)->setWall(WEST, true);
        getTile(width / 2 - 1, height / 2 - 1)->setWall(EAST, true);

        getTile(width / 2, height / 2)->setWall(NORTH, true);
        getTile(width / 2, height / 2 + 1)->setWall(SOUTH, true);

        getTile(width / 2, height / 2)->setWall(EAST, true);
        getTile(width / 2 + 1, height / 2)->setWall(WEST, true);

        getTile(width / 2, height / 2)->setWall(WEST, true);
        getTile(width / 2 - 1, height / 2)->setWall(EAST, true);

        switch (rand() / (1 + RAND_MAX / 6)){
        case 0:
            getTile(width / 2, height / 2 - 1)->setWall(SOUTH, false);
            getTile(width / 2, height / 2 - 2)->setWall(NORTH, false);
            break;
        case 1:
            getTile(width / 2, height / 2 - 1)->setWall(EAST, false);
            getTile(width / 2 + 1, height / 2 - 1)->setWall(WEST, false);
            break;
        case 2:
            getTile(width / 2, height / 2 - 1)->setWall(WEST, false);
            getTile(width / 2 - 1, height / 2 - 1)->setWall(EAST, false);
            break;
        case 3:
            getTile(width / 2, height / 2)->setWall(NORTH, false);
            getTile(width / 2, height / 2 + 1)->setWall(SOUTH, false);
            break;
        case 4:
            getTile(width / 2, height / 2)->setWall(EAST, false);
            getTile(width / 2 + 1, height / 2)->setWall(WEST, false);
            break;
        case 5:
            getTile(width / 2, height / 2)->setWall(WEST, false);
            getTile(width / 2 - 1, height / 2)->setWall(EAST, false);
            break;
        }
    }

    // Break a random wall leading to center group
    if (height % 2 == 0 && width % 2 == 0){

        getTile(width / 2 - 1, height / 2 - 1)->setWall(WEST, true);
        getTile(width / 2 - 2, height / 2 - 1)->setWall(EAST, true);

        getTile(width / 2 - 1, height / 2 - 1)->setWall(SOUTH, true);
        getTile(width / 2 - 1, height / 2 - 2)->setWall(NORTH, true);

        getTile(width / 2 - 1, height / 2)->setWall(NORTH, true);
        getTile(width / 2 - 1, height / 2 + 1)->setWall(SOUTH, true);

        getTile(width / 2 - 1, height / 2)->setWall(WEST, true);
        getTile(width / 2 - 2, height / 2)->setWall(EAST, true);

        getTile(width / 2, height / 2 - 1)->setWall(EAST, true);
        getTile(width / 2 + 1, height / 2 - 1)->setWall(WEST, true);

        getTile(width / 2, height / 2 - 1)->setWall(SOUTH, true);
        getTile(width / 2, height / 2 - 2)->setWall(NORTH, true);

        getTile(width / 2, height / 2)->setWall(NORTH, true);
        getTile(width / 2, height / 2 + 1)->setWall(SOUTH, true);

        getTile(width / 2, height / 2)->setWall(EAST, true);
    getTile(width / 2 + 1, height / 2)->setWall(WEST, true);


        switch (rand() / (1 + RAND_MAX / 8)){
        case 0:
            getTile(width / 2 - 1, height / 2 - 1)->setWall(WEST, false);
            getTile(width / 2 - 2, height / 2 - 1)->setWall(EAST, false);
            break;
        case 1:
            getTile(width / 2 - 1, height / 2 - 1)->setWall(SOUTH, false);
            getTile(width / 2 - 1, height / 2 - 2)->setWall(NORTH, false);
            break;
        case 2:
            getTile(width / 2 - 1, height / 2)->setWall(NORTH, false);
            getTile(width / 2 - 1, height / 2 + 1)->setWall(SOUTH, false);
            break;
        case 3:
            getTile(width / 2 - 1, height / 2)->setWall(WEST, false);
            getTile(width / 2 - 2, height / 2)->setWall(EAST, false);
            break;
        case 4:
            getTile(width / 2, height / 2 - 1)->setWall(EAST, false);
            getTile(width / 2 + 1, height / 2 - 1)->setWall(WEST, false);
            break;
        case 5:
            getTile(width / 2, height / 2 - 1)->setWall(SOUTH, false);
            getTile(width / 2, height / 2 - 2)->setWall(NORTH, false);
            break;
        case 6:
            getTile(width / 2, height / 2)->setWall(NORTH, false);
            getTile(width / 2, height / 2 + 1)->setWall(SOUTH, false);
            break;
        case 7:
            getTile(width / 2, height / 2)->setWall(EAST, false);
            getTile(width / 2 + 1, height / 2)->setWall(WEST, false);
            break;
        }
    }

    // Once the walls are assigned, we can assign neighbors
    assignNeighbors();
}

void Maze::saveMaze(std::string mazeFile){
    
    // Create the stream
    std::ofstream file(mazeFile.c_str());

    if (file.is_open()){

        // Very primitive, but will work
        for (int x = 0; x <  getWidth(); x += 1){
            for (int y = 0; y < getHeight(); y += 1){
                file << x << " " << y;
                for (int k = 0; k < 4; k += 1){
                    file << " " << (getTile(x, y)->isWall(k) ? 1 : 0);
                }
                file << std::endl;
            }
        }

        file.close();
    }
}

void Maze::loadMaze(std::string mazeFile){

    // Create the stream
    std::ifstream file(mazeFile.c_str());

    // Initialize a string variable
    std::string line("");

    if (file.is_open()){

        // Very primitive, but will work
        while (getline(file, line)){
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                 std::back_inserter<std::vector<std::string> >(tokens));
            for (int i = 0; i < 4; i += 1){ // Set the values of all of the walls
                getTile(atoi(tokens.at(0).c_str()), atoi(tokens.at(1).c_str()))
                      ->setWall(i, atoi(tokens.at(2+i).c_str()));
            }
        }
    
        file.close();

        // Once the walls are assigned, we can assign neighbors
        assignNeighbors();
    }
}

void Maze::assignNeighbors(){
    for (int x = 0; x < getWidth(); x += 1){
        for (int y = 0; y < getHeight(); y += 1){

            Tile* tile = getTile(x, y);

            // First we ensure that we have a fresh (empty) list
            tile->resetNeighbors();

            // Then we assign new neighbors
            if (!tile->isWall(NORTH)){
                tile->addNeighbor(getTile(x, y+1));
            }
            if (!tile->isWall(EAST)){
                tile->addNeighbor(getTile(x+1, y));
            }
            if (!tile->isWall(SOUTH)){
                tile->addNeighbor(getTile(x, y-1));
            }
            if (!tile->isWall(WEST)){
                tile->addNeighbor(getTile(x-1, y));
            }
        }        
    }        
}

void Maze::printDistances(){
    std::cout << std::endl;
    for (int y = getHeight()-1; y >= 0; y -= 1){
        for (int x = 0; x < getWidth(); x += 1){
            if (getTile(x, y)->getDistance() < 100){
                std::cout << " ";
                if (getTile(x, y)->getDistance() < 10){
                    std::cout << " ";
                }
            }
            std::cout << getTile(x, y)->getDistance() << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<bool> Maze::solveShortestPath(){

    // Solves the maze, assigns tiles that are part of the shortest path
    std::vector<Tile*> sp = findPathToCenter();
    for (int i = 0; i < sp.size(); i += 1){
        getTile(sp.at(i)->getX(), sp.at(i)->getY())->setPosp(true);
    }

    // Returns whether or not the maze is solvable and whether or not
    // it satisfies the minimum number of steps
    std::vector<bool> conditions;
    conditions.push_back(getClosestCenterTile()->getDistance() < MAX_DISTANCE);
    conditions.push_back(getClosestCenterTile()->getDistance() > MIN_MAZE_STEPS);
    return conditions;
}

std::vector<Tile*> Maze::findPath(int x1, int y1, int x2, int y2){
    setDistancesFrom(x1, x2);
    return backtrace(getTile(x2, y2));
}

std::vector<Tile*> Maze::findPathToCenter(){
    setDistancesFrom(0, 0);
    return backtrace(getClosestCenterTile());
}

std::vector<Tile*> Maze::backtrace(Tile* end){

    // The vector to hold the solution nodes
    std::vector<Tile*> path;

    // Start at the ending node and simply trace back through the values
    std::queue<Tile*> queue;
    queue.push(end);
    path.push_back(end);

    while (!queue.empty()){
        Tile* node = queue.front();
        queue.pop(); // Removes the element
        std::vector<Tile*> neighbors = node->getNeighbors();
        for (int i = 0; i < neighbors.size(); i += 1){
            if (neighbors.at(i)->getDistance() == node->getDistance() - 1){
                path.push_back(neighbors.at(i));
                queue.push(neighbors.at(i));
            }
        }
    }

    return path;
}

void Maze::setDistancesFrom(int x, int y){
    
    // Ensure that the nodes are fresh every time this function is called
    for (int x = 0; x < getWidth(); x += 1){
        for (int y = 0; y < getHeight(); y += 1){
            Tile* tile = getTile(x, y);
            tile->setDistance(MAX_DISTANCE);
            tile->setExplored(false);
            tile->setPosp(false);
        }
    }

    // Queue for BFS
    std::queue<Tile*> queue;
    queue.push(getTile(x, y));
    getTile(x, y)->setDistance(0);
    getTile(x, y)->setExplored(true);

    while (!queue.empty()){
        Tile* node = queue.front();
        queue.pop(); // Removes the element
        std::vector<Tile*> neighbors = node->getNeighbors();
        for (int i = 0; i < neighbors.size(); i += 1){
            if (!neighbors.at(i)->getExplored()){
                neighbors.at(i)->setDistance(node->getDistance() + 1); 
                queue.push(neighbors.at(i));
                neighbors.at(i)->setExplored(true);
            }
        }
    }
}

Tile* Maze::getClosestCenterTile(){

    if (getWidth() > 0 && getHeight() > 0){
    
        int midWidth = getWidth()/2;
        int midHeight = getHeight()/2;
        Tile* closest = getTile(midWidth, midHeight);

        if (getWidth() % 2 == 0){
            if (getTile(midWidth-1, midHeight)->getDistance() < closest->getDistance()){
                closest = getTile(midWidth-1, midHeight);
            }
        }
        if (getHeight() % 2 == 0){
            if (getTile(midWidth, midHeight-1)->getDistance() < closest->getDistance()){
                closest = getTile(midWidth, midHeight-1);
            }
        }
        if (getWidth() % 2 == 0 && getHeight() % 2 == 0){
            if (getTile(midWidth-1, midHeight-1)->getDistance() < closest->getDistance()){
                closest = getTile(midWidth-1, midHeight-1);
            }
        }
        
        return closest;
    }
    
    // If either the width or the height is zero return NULL
    return NULL;
}

} // namespace sim
