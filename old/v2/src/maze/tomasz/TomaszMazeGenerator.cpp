#include "TomaszMazeGenerator.h"

#include <stack>
#include <cmath>
#include <queue>
#include <iostream>

namespace tomasz {

void TomaszMazeGenerator::generate(int mazeWidth, int mazeHeight, sim::MazeInterface* mazeInterface) {
    m_mazeInterface = mazeInterface;
    generateMaze(mazeWidth, mazeHeight);
    convertToBasicMaze(mazeInterface);
}

void TomaszMazeGenerator::convertToBasicMaze(sim::MazeInterface* mazeInterface) {
    for (int x = 0; x < m_width; x += 1) {
        for (int y = 0; y < m_height; y += 1) {
            for (char c : {'n', 'e', 's', 'w'}) {
                switch (c) {
                    case 'n':
                        mazeInterface->setWall(x, y, 'n', getTile(x, y)->walls.at(NORTH));
                        break;
                    case 'e':
                        mazeInterface->setWall(x, y, 'e', getTile(x, y)->walls.at(EAST));
                        break;
                    case 's':
                        mazeInterface->setWall(x, y, 's', getTile(x, y)->walls.at(SOUTH));
                        break;
                    case 'w':
                        mazeInterface->setWall(x, y, 'w', getTile(x, y)->walls.at(WEST));
                        break;
                        
                }
            }
        }
    }
}

void TomaszMazeGenerator::generateMaze(int mazeWidth, int mazeHeight) {

    // Declare width and height locally to reduce function calls
    m_width = mazeWidth;
    m_height = mazeHeight;
    m_direction = UNDEFINED;
    
    initializeMaze();
    
    getTile(0, 0)->explored = true; // Put the first cell on stack
    getTile(0, 0)->distanceFromStart = 0; // Cell 0
    TomMazeGenTile* t = getTile(0,0);
    m_stack.push(t);
                        
    const float deadEndBreak = tomDeadEndBreakChance;
    
    // Continue to DFS until we've explored every Tile
    while (m_stack.empty() == false) {
        // Grab and unpack the top thing from the stack
        TomMazeGenTile* current = m_stack.top();
        
        const int xPos = current->column;
        const int yPos = current->row; 
        
        // Keep track of the next possible movements

        std::map<Direction, bool> choices = {
            {NORTH, isValidUnexploredMove(xPos, yPos, NORTH)},
            {EAST,  isValidUnexploredMove(xPos, yPos, EAST) },
            {SOUTH, isValidUnexploredMove(xPos, yPos, SOUTH)},
            {WEST,  isValidUnexploredMove(xPos, yPos, WEST) }
        };
        
        int possible = 0;
        
        for (auto &choice : choices) {
            if (choice.second) {
                possible += 1; // Count how many cells can be moved to
            }
        }

        // If the current cell has no more paths forward, we're done
        if (possible == 0) {
            m_stack.pop(); // Pop the tile of the stack, this will prompt the algorith to go back
                           // until a cell has possible moves
            //std::cout << "HERE2" << std::endl;
                                      
            if (m_direction != UNDEFINED) { // We just reached the end of a path. lets break a wall with probability P
                if (m_mazeInterface->getRandom() <= deadEndBreak) { // break the wall to the most disjoint
                    
                    updateDistanceFromStart(xPos, yPos);
                    breakGradientWall(xPos, yPos); // break the wall towards greatest gradient,
                                                          // this will result in more open mazes
                }

            }
            m_direction = UNDEFINED; // Once we start backtracing there is no last move
        } else {
                    
            const float xCenterDistance = (abs(xPos - m_width / 2.0) * 2.0) / (m_width-1); //ratios of how far you are from the center
            const float yCenterDistance = (abs(yPos - m_height / 2.0) * 2.0) / (m_height-1);
            
            const static float straightFactor = tomStraightFactor;

            // the chance the algorithm will continue same direction of tavel
            // if zero the maze will always turn. if 1 maze will be longest possible
            // Currently proprtional to distance from center <see below>

            const float moveConst = straightFactor * std::max(xCenterDistance, yCenterDistance);
            
            Direction directionToMove = getDirectionToMove(moveConst, choices);
            
            m_direction = moveTowardDirection(xPos, yPos, directionToMove);
        }
    }

    breakGradientWalls(); // Break one wall down with biggest gradient across it
    
    updateDistanceFromStart();
    
    pathIntoCenter(); // Now break down one of those wall at random
}

void TomaszMazeGenerator::pathIntoCenter() {
    // These center variables will always be the lower left corner of
    // the center.  If a side is odd, the division will yeild X.5,
    // this rounded will result in middle.  Middle square of 3 is 2.
    // 3 / 2 = 1.5 -> 2         4 / 2 = 2 -> 2 
    
    Direction directions[4] = {NORTH, EAST, SOUTH, WEST};
    
    int xCenter = std::round(static_cast<double>(m_width) / 2.0) - 1; // 0 based vector
    int yCenter = std::round(static_cast<double>(m_height) / 2.0) - 1;
    
    int greatestGradient = 0;
    int xPosOfGreatest = xCenter;
    int yPosOfGreatest = yCenter;
    
    if ( (m_width % 2 == 0) && (m_height % 2 == 0) ){
        for(int z = 0; z < 4; z++) {
            int currentCellDist = getTile(xCenter + 1, yCenter + 1)->distanceFromStart;
            if (isValidExploredTile(xCenter + 1, yCenter + 1, directions[z]) &&
              abs( getTile(xCenter + 1, yCenter + 1, directions[z])->distanceFromStart - currentCellDist ) >
              greatestGradient) {

              greatestGradient = abs(getTile(xCenter + 1, yCenter + 1, directions[z])->
                  distanceFromStart - currentCellDist);
              xPosOfGreatest = xCenter + 1;
              yPosOfGreatest = yCenter + 1;
            }
        }
    }
    
    if (m_width % 2 == 0) {
        for(int z = 0; z < 4; z++) {
            int currentCellDist = getTile(xCenter + 1, yCenter)->distanceFromStart;
            if (isValidExploredTile(xCenter + 1, yCenter, directions[z]) &&
              abs( getTile(xCenter + 1, yCenter, directions[z])->distanceFromStart - currentCellDist ) >
              greatestGradient) {

              greatestGradient = abs(getTile(xCenter + 1, yCenter, directions[z])->
                  distanceFromStart - currentCellDist);
              xPosOfGreatest = xCenter + 1;
              yPosOfGreatest = yCenter;
            }
        }
    }
    
    if (m_height % 2 == 0) {
        for(int z = 0; z < 4; z++) {
            int currentCellDist = getTile(xCenter, yCenter + 1)->distanceFromStart;
            if (isValidExploredTile(xCenter, yCenter + 1, directions[z]) &&
              abs( getTile(xCenter, yCenter + 1, directions[z])->distanceFromStart - currentCellDist ) >
              greatestGradient) {

              greatestGradient = abs(getTile(xCenter, yCenter + 1, directions[z])->
                  distanceFromStart - currentCellDist);
              xPosOfGreatest = xCenter;
              yPosOfGreatest = yCenter + 1;
            }
        }
    }
    
    breakGradientWall(xPosOfGreatest, yPosOfGreatest);
}

void TomaszMazeGenerator::breakGradientWalls() {
    
    Direction directions[4] = {NORTH, EAST, SOUTH, WEST};
    
    for(int i = 0; i < tomGradientWallBreaks; i++) {
        
        int greatestGradient = 0;
        int xPosOfGreatest = 0;
        int yPosOfGreatest = 0;
        
        for(int x = 0; x < m_width; x++) {
            for(int y = 0; y < m_height; y++) {
                updateDistanceFromStart(x, y); // compute distances not from start, but from cell x,y
                for(int z = 0; z < 4; z++) {
                    if(getTile(x, y)->isCenter == true) { continue; }
                    if(getTile(x,y)->row == 0 && getTile(x,y)->column == 0) { continue; }
                    
                    int currentCellDist = getTile(x, y)->distanceFromStart;
                    
                    if (isValidExploredTile(x, y, directions[z]) && // We do not want center
                        abs( getTile(x, y, directions[z])->distanceFromStart - currentCellDist ) >
                        greatestGradient) {
                        
                        greatestGradient = abs(getTile(x, y, directions[z])->
                            distanceFromStart - currentCellDist);
                        xPosOfGreatest = x;
                        yPosOfGreatest = y;
                    }
                }
            }
        }
        
        updateDistanceFromStart(xPosOfGreatest, yPosOfGreatest);
        // This will figure out the direction of the greatest gradient by itself
        breakGradientWall(xPosOfGreatest, yPosOfGreatest);
    }
}

Direction TomaszMazeGenerator::moveTowardDirection(int xPos, int yPos, Direction directionToMove){
    // Set the tile in the direction we move to explored and attach the number
    // Break down the wall between the current cell and that cell
    // And push it unto the stack
    
    Direction exploreDIR = UNDEFINED; // make sure we do not return garbage data
    
    switch (directionToMove) {
        case NORTH:
            setWall(xPos, yPos, NORTH, false);
            getTile(xPos, yPos, NORTH)->explored = true;
            m_stack.push(getTile(xPos, yPos, NORTH)); // Push next cell unto stack
            exploreDIR = NORTH;
            break;
        case EAST:
            setWall(xPos, yPos, EAST, false);
            getTile(xPos, yPos, EAST)->explored = true;
            m_stack.push(getTile(xPos, yPos, EAST));
            exploreDIR = EAST;
            break;
        case SOUTH:
            setWall(xPos, yPos, SOUTH, false);
            getTile(xPos, yPos, SOUTH)->explored = true;
            m_stack.push(getTile(xPos, yPos, SOUTH));
            exploreDIR = SOUTH;
            break;
        case WEST:
            setWall(xPos, yPos, WEST, false);
            getTile(xPos, yPos, WEST)->explored = true;
            m_stack.push(getTile(xPos, yPos, WEST));
            exploreDIR = WEST;
            break;
        case UNDEFINED:
            std::cout << "This should never happen in " << __FILE__ << ", line: " << __LINE__ << std::endl;
            break;
    }
    
    return exploreDIR;
}

void TomaszMazeGenerator::updateDistanceFromStart(int xPos, int yPos){

    // Note: In a normal recursive backtracer maze generation algorithm, the distance
    // from the start cell is just the size of the stack.  Since I added wall breaking
    // in certain scenarios, this method is no longer reliable.  Thus, this method,
    // computes the distance from start recursivley by a breadth first search.  The
    // distance from start of a cell is one more than the distance from start of the cell
    // it came from.  Also, we do not have to check if a cell that we are about to visit
    // has been explored, since it will be walled off completely.  Cells start with all
    // walls up.
    
     for (int x = 0; x < m_width; x++) {
        std::vector<TomMazeGenTile> column;
        for (int y = 0; y < m_height; y++) {
             getTile(x, y)->distanceFromStart = -1; // reset all distances
        }
    }
    
    std::queue<TomMazeGenTile*> pathQueue;
    
    std::map<Direction, bool> visited = {
        {NORTH, false},
        {EAST,  false},
        {SOUTH, false},
        {WEST,  false}
    };
    
    TomMazeGenTile* t = getTile(xPos, yPos);
    t->distanceFromStart = 0; // tile xPos, yPos is start
    pathQueue.push(t);
    
    while(pathQueue.empty() == false){
        TomMazeGenTile* tile = pathQueue.front();
        pathQueue.pop();
        
        int xCurrentPos = tile->column;
        int yCurrentPos = tile->row;
        
        int newDistanceFromStart = tile->distanceFromStart + 1;
        
        if(tile->walls[NORTH] == false && getTile(xCurrentPos, yCurrentPos, NORTH)->distanceFromStart == -1) {
            TomMazeGenTile* tileToAdd = getTile(xCurrentPos, yCurrentPos, NORTH);
            tileToAdd->distanceFromStart = newDistanceFromStart;
            pathQueue.push(tileToAdd);   
             
        }
        
        if(tile->walls[EAST] == false && getTile(xCurrentPos, yCurrentPos, EAST)->distanceFromStart == -1) {
            TomMazeGenTile* tileToAdd = getTile(xCurrentPos, yCurrentPos, EAST);
            tileToAdd->distanceFromStart = newDistanceFromStart;
            pathQueue.push(tileToAdd);    
        }
        
        if(tile->walls[SOUTH] == false && getTile(xCurrentPos, yCurrentPos, SOUTH)->distanceFromStart == -1) {
            TomMazeGenTile* tileToAdd = getTile(xCurrentPos, yCurrentPos, SOUTH);
            tileToAdd->distanceFromStart = newDistanceFromStart;
            pathQueue.push(tileToAdd);    
        }
        
        if(tile->walls[WEST] == false && getTile(xCurrentPos, yCurrentPos, WEST)->distanceFromStart == -1) {
            TomMazeGenTile* tileToAdd = getTile(xCurrentPos, yCurrentPos, WEST);
            tileToAdd->distanceFromStart = newDistanceFromStart;
            pathQueue.push(tileToAdd);    
        }
        
        if((xCurrentPos == xPos)     && (yCurrentPos + 1 == yPos)) { visited[NORTH] = true; }
        if((xCurrentPos + 1 == xPos) && (yCurrentPos == yPos))     { visited[EAST]  = true; }
        if((xCurrentPos == xPos)     && (yCurrentPos - 1 == yPos)) { visited[SOUTH] = true; }
        if((xCurrentPos - 1 == xPos) && (yCurrentPos == yPos))     { visited[WEST]  = true; }
        
        if((visited[NORTH] == true) &&
           (visited[EAST]  == true) &&
           (visited[SOUTH] == true) &&
           (visited[WEST]  == true) &&
           ((xPos != 0) || (yPos != 0)) ){
           break; // If we are doing breadth search from a cell, we only really NEED the distance
                  // values of all cells adjacent to that cell.  No sense in wasting time
        }
    }
    
}

Direction TomaszMazeGenerator::getDirectionToMove(float moveConst, std::map<Direction, bool> choices){

    //if previous move exists, and repeating it will yeild a valid move AND its not the only valid move

    int possible = 0;
        
    for(auto &choice : choices) {
        if(choice.second == true){
            possible++; // Count how many cells can be moved to
        }
    }   
        
    Direction directionToMove;
    
    if (m_direction != UNDEFINED && choices[m_direction] != false && possible != 1) {
        if (m_mazeInterface->getRandom() <= moveConst) {  // if random variable is below threshold move in last direction
            directionToMove = m_direction;
        } else {
            auto item = choices.begin();
            std::advance( item, std::floor(m_mazeInterface->getRandom() * 4 ));
            directionToMove =  item->first;

            while (choices[directionToMove] == false || directionToMove == m_direction) { // but the last moved direction
                item = choices.begin();
                std::advance( item, std::floor(m_mazeInterface->getRandom() * 4 ));
                directionToMove = item->first;         
            }
        }
    }
    else { // otherwise just move in a random viable direction
        auto item = choices.begin();
        std::advance( item, std::floor(m_mazeInterface->getRandom() * 4 ));
        directionToMove = item->first;   

        while (choices[directionToMove] == false) { // but the last moved direction
            item = choices.begin();
            std::advance( item, std::floor(m_mazeInterface->getRandom() * 4 ));
            directionToMove = item->first;
        }
    }
    
    return directionToMove;
}
void TomaszMazeGenerator::breakGradientWall(int xPos, int yPos) {

    int currentCellDist = getTile(xPos, yPos)->distanceFromStart;
    int biggestDifference = 0;
    Direction cellToBreak = UNDEFINED;
    const static float breakThreshold = tomDeadEndBreakThreshold;
        
    // If cell to the North is valid and explored
    if (isValidExploredTile(xPos, yPos, NORTH)) {
        if (abs(getTile(xPos, yPos, NORTH)->distanceFromStart - currentCellDist) > biggestDifference) {
            biggestDifference = abs(getTile(xPos, yPos, NORTH)->distanceFromStart - currentCellDist);
            cellToBreak = NORTH;
        }
    }
    // If cell to the East is valid and explored
    if (isValidExploredTile(xPos, yPos, EAST)) {
        if (abs(getTile(xPos, yPos, EAST)->distanceFromStart - currentCellDist) > biggestDifference) {
            biggestDifference = abs(getTile(xPos, yPos, EAST)->distanceFromStart - currentCellDist);
            cellToBreak = EAST;
        }
    }
    // If cell to the South is valid and explored
    if (isValidExploredTile(xPos, yPos, SOUTH)) {
        if (abs(getTile(xPos, yPos, SOUTH)->distanceFromStart - currentCellDist) > biggestDifference) {
            biggestDifference = abs(getTile(xPos, yPos, SOUTH)->distanceFromStart - currentCellDist);
            cellToBreak = SOUTH;
        }
    }
    // If cell to the West is valid and explored
    if (isValidExploredTile(xPos, yPos, WEST)) {
        if (abs(getTile(xPos, yPos, WEST)->distanceFromStart - currentCellDist) > biggestDifference) {
            biggestDifference = abs(getTile(xPos, yPos, WEST)->distanceFromStart - currentCellDist);
            cellToBreak = WEST;
        }
    }
    //biggestDifference = 0;
    if (biggestDifference > breakThreshold) {
        switch (cellToBreak) { // Break wall across greates gradiant
        case NORTH:
            setWall(xPos, yPos, NORTH, false);
            break;
        case SOUTH:
            setWall(xPos, yPos, SOUTH, false);
            break;
        case EAST:
            setWall(xPos, yPos, EAST, false);
            break;
        case WEST:
            setWall(xPos, yPos, WEST, false);
            break;
        case UNDEFINED:
            std::cout << "This should never happen in " << __FILE__ << ", line: " << __LINE__ << std::endl;
            break;
        }
    }
}

/*std::map<Direction, bool> TomaszMazeGenerator::getValidMoves(int xPos, int yPos) {
    
    std::map<Direction, bool> choices = {
        {NORTH, isValidUnexploredMove(xPos, yPos, NORTH)},
        {EAST,  isValidUnexploredMove(xPos, yPos, EAST) },
        {SOUTH, isValidUnexploredMove(xPos, yPos, SOUTH)},
        {WEST,  isValidUnexploredMove(xPos, yPos, WEST) }
    };
    
    std::map<Direction, bool> choices;
        
    choices[NORTH] = isValidUnexploredMove(xPos, yPos, NORTH);
    choices[EAST] = isValidUnexploredMove(xPos, yPos, EAST);
    choices[SOUTH] = isValidUnexploredMove(xPos, yPos, SOUTH);
    choices[WEST] = isValidUnexploredMove(xPos, yPos, WEST);
    
    return choices;
}*/

bool TomaszMazeGenerator::isValidUnexploredMove(int x, int y, Direction direction) {
    if (direction == NORTH) { y++; }
    if (direction == EAST)  { x++; }
    if (direction == SOUTH) { y--; }
    if (direction == WEST)  { x--; }
    
    if ( (x >= 0)      && (y >= 0) &&
         (x < m_width) && (y < m_height) &&
         (getTile(x, y)->explored == false) &&
         (getTile(x, y)->isCenter == false) ) {
        return true;
    } else {
        return false;
    }
}

bool TomaszMazeGenerator::isValidExploredTile(int x, int y, Direction direction) {
    if (direction == NORTH) { y++; }
    if (direction == EAST)  { x++; }
    if (direction == SOUTH) { y--; }
    if (direction == WEST)  { x--; }
    
    if ( (x >= 0)      && (y >= 0) &&
         (x < m_width) && (y < m_height) &&
         (getTile(x, y)->row != 0 || getTile(x, y)->column != 0) &&
         (getTile(x, y)->explored == true) &&
         (getTile(x, y)->isCenter == false) ) {
        return true;
    } else {
        return false;
    }
}

void TomaszMazeGenerator::initializeMaze() {

    m_maze.clear(); // In case the calling code called generate twice.
                    // We want to clear the vector, to not push twice
                    // the maze.
    
    for (int x = 0; x < m_width; x++) {
        std::vector<TomMazeGenTile> column;
        for (int y = 0; y < m_height; y++) {
            TomMazeGenTile tile;
            tile.row = y;
            tile.column = x;
            tile.walls[NORTH] = true;
            tile.walls[EAST]  = true;
            tile.walls[SOUTH] = true;
            tile.walls[WEST]  = true;
            tile.isCenter = false;
            tile.explored = false;
            tile.distanceFromStart = -1;
            column.push_back(tile);
        }
        m_maze.push_back(column);
    }
    
    makeCenter();
}

void TomaszMazeGenerator::makeCenter(){

    // These center variables will always be the lower left corner of
    // the center.  If a side is odd, the division will yeild X.5,
    // this rounded will result in middle.  Middle square of 3 is 2.
    // 3 / 2 = 1.5 -> 2         4 / 2 = 2 -> 2 
    
    int xCenter = std::round(static_cast<double>(m_width) / 2.0) - 1; // 0 based vector
    int yCenter = std::round(static_cast<double>(m_height) / 2.0) - 1;
    
    getTile(xCenter, yCenter)->isCenter = true; // This is always the middle
    
    if ( (m_width % 2 == 0) && (m_height % 2 == 0) ){
        setWall(xCenter + 1, yCenter + 1, SOUTH, false); // Top right corner of
        setWall(xCenter + 1, yCenter + 1, WEST, false);  // square middle
        getTile(xCenter + 1, yCenter + 1)->isCenter = true;
    }
    
    if (m_width % 2 == 0) {
        setWall(xCenter, yCenter, EAST, false);
        getTile(xCenter, yCenter, EAST)->isCenter = true;
    }
    
    if (m_height % 2 == 0) {
        setWall(xCenter, yCenter, NORTH, false);
        getTile(xCenter, yCenter, NORTH)->isCenter = true;
    }
}

void TomaszMazeGenerator::setWall(int x, int y, Direction direction, bool value) {

    switch (direction) {
        case NORTH:
            getTile(x, y)->walls[NORTH] = value;
            getTile(x, y, NORTH)->walls[SOUTH] = value;
            break;
        case SOUTH:
            getTile(x, y)->walls[SOUTH] = value;
            getTile(x, y, SOUTH)->walls[NORTH] = value;
            break;
        case EAST:
            getTile(x, y)->walls[EAST] = value;
            getTile(x, y, EAST)->walls[WEST] = value;
            break;
        case WEST:
            getTile(x, y)->walls[WEST] = value;
            getTile(x, y, WEST)->walls[EAST] = value;
            break;
        case UNDEFINED:
            std::cout << "This should never happen in " << __FILE__ << ", line: " << __LINE__ << std::endl;
            break;
    }
}

TomaszMazeGenerator::TomMazeGenTile* TomaszMazeGenerator::getTile(int x, int y, Direction direction){
    if (direction == NORTH) { y++; }
    if (direction == EAST)  { x++; }
    if (direction == SOUTH) { y--; }
    if (direction == WEST)  { x--; }
    
    return &m_maze.at(x).at(y);
}

} // namespace tomasz
