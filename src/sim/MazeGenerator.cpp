#include "MazeGenerator.h"

#include "Param.h"
#include "SimUtilities.h"

namespace sim {

std::vector<std::vector<BasicTile>> MazeGenerator::generateMaze(Algorithm algo) {

    // Get the maze based on the choice of algorithm
    std::vector<std::vector<BasicTile>> maze;
    switch (algo) {
        case TOMASZ:
            maze = tomasz();
            break;
        case RANDOM:
            maze = random();
            break;
    }
    return maze;
}

std::vector<std::vector<BasicTile>> MazeGenerator::random() {

    // Create the maze that will be returned
    std::vector<std::vector<BasicTile>> maze;
    for (int x = 0; x < P()->generatedMazeWidth(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < P()->generatedMazeHeight(); y += 1) {
            BasicTile tile;
            tile.x = x;
            tile.y = y;
            column.push_back(tile);
        }
        maze.push_back(column);
    }

    // Percentage chance any one wall will exist
    float wallProb = 0.40;
    for (int x = 0; x < maze.size(); x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            for (Direction direction : DIRECTIONS) {
                bool wallExists = (SimUtilities::getRandom() <= wallProb);
                switch (direction) {
                    case NORTH:
                        if (y + 1 < P()->generatedMazeHeight()) {
                            maze.at(x).at(y).walls[NORTH] = wallExists;
                            maze.at(x).at(y + 1).walls[SOUTH] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[NORTH] = true;
                        }
                        break;
                    case EAST:
                        if (x + 1 < P()->generatedMazeWidth()) {
                            maze.at(x).at(y).walls[EAST] = wallExists;
                            maze.at(x + 1).at(y).walls[WEST] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[EAST] = true;
                        }
                        break;
                    case SOUTH:
                        if (y > 0) {
                            maze.at(x).at(y).walls[SOUTH] = wallExists;
                            maze.at(x).at(y - 1).walls[NORTH] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[SOUTH] = true;
                        }
                        break;
                    case WEST:
                        if (x > 0) {
                            maze.at(x).at(y).walls[WEST] = wallExists;
                            maze.at(x - 1).at(y).walls[EAST] = wallExists;
                        }
                        else {
                            maze.at(x).at(y).walls[WEST] = true;
                        }
                        break;
                }
            }
        }
    }
    return maze;
}

std::vector<std::vector<BasicTile>> MazeGenerator::tomasz() {
    std::vector<std::vector<BasicTile>> maze;
    return maze;
}

#if(0)
void Maze::tom_random() {

    // TODO: USE CONST TILE WHERE POSSIBLE

    /*
    // Declare width and height locally to reduce function calls
    int width = getWidth();
    int height = getHeight();

    initializeMaze(); // Initialize all walls to 1 and set middle as a distance of 120

    // We'll use a stack to do a DFS
    std::stack<Tile*> toExplore;

    getTile(0, 0)->setExplored(true); // Put the first cell on stack
    getTile(0, 0)->setDistance(0); // Cell 0
    const? Tile* t = getTile(0,0);
    toExplore.push(t);

    int exploreDIR = -1; //last direction the mouse moved. -1 if DNE

    float moveConst = 0; // the chance the algorithm will continue same direction of tavel
                         // if zero the maze will always turn. if 1 maze will be longest possible
                        // Currently proprtional to distance from center <see below>

    int dirThreshold = RAND_MAX * moveConst; // the threshold which the random number will be
                                                      // compared too

    // Continue to DFS until we've explored every Tile
    while (!toExplore.empty()) {
        // Grab and unpack the top thing from the stack
        Tile* current = toExplore.top();

        int x_pos = current->getX();
        int y_pos = current->getY();

        float xDelta = (abs(x_pos - width / 2.0) * 2.0) / (width-1); //ratios of how far you are from the center
        float yDelta = (abs(y_pos - height / 2.0) * 2.0) / (height-1);

        float avgDelta = std::max(xDelta, yDelta) * 0.85; // * 0.85 gives good compromise betwwen straight and turned

        moveConst = 1 * avgDelta; // the chance the algorithm will continue same direction of tavel is proportional
                                  // to the max distance of the mouse to the center on any axis.

        dirThreshold = static_cast<float>(RAND_MAX) * moveConst;

        // Keep track of the next possible movements N = 0, E = 1, S = 2, W =3
        bool choices[4] = {0,0,0,0};

        // If cell to the North is valid and unexplored
        if ((y_pos + 1 < getHeight()) && !getTile(x_pos, y_pos + 1)->getExplored()) {
            choices[NORTH] = true;
        }
        // If cell to the East is valid and unexplored
        if ((x_pos + 1 < getWidth()) && !getTile(x_pos + 1, y_pos)->getExplored()) {
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
            if (exploreDIR != -1) { // We just reached the end of a path. lets break a wall with probability P
                if (rand() <= RAND_MAX * .75) { // HULK SMASH the wall that will connect two most disjoint

                    int currentCellDist = getTile(x_pos, y_pos)->getDistance();
                    int biggestDifference = 0;
                    int cellToBreak = 0; // NORTH, SOUTH, EAST, WEST

                    // If cell to the North is valid and explored
                    if ((y_pos + 1 < getHeight()) && getTile(x_pos, y_pos + 1)->getExplored()) {
                        if (abs(getTile(x_pos, y_pos + 1)->getDistance() - currentCellDist) > biggestDifference) {
                            biggestDifference = abs(getTile(x_pos, y_pos + 1)->getDistance() - currentCellDist);
                            cellToBreak = NORTH;
                        }
                    }
                    // If cell to the East is valid and explored
                    if ((x_pos + 1 < getWidth()) && getTile(x_pos + 1, y_pos)->getExplored()) {
                        if (abs(getTile(x_pos + 1, y_pos)->getDistance() - currentCellDist) > biggestDifference) {
                            biggestDifference = abs(getTile(x_pos + 1, y_pos)->getDistance());
                            cellToBreak = EAST;
                        }
                    }
                    // If cell to the South is valid and explored
                    if ((y_pos - 1 >= 0) && getTile(x_pos, y_pos - 1)->getExplored()) {
                        if (abs(getTile(x_pos, y_pos - 1)->getDistance() - currentCellDist) > biggestDifference) {
                            biggestDifference = abs(getTile(x_pos, y_pos - 1)->getDistance());
                            cellToBreak = SOUTH;
                        }
                    }
                    // If cell to the West is valid and explored
                    if ((x_pos - 1 >= 0) && getTile(x_pos - 1, y_pos)->getExplored()) {
                        if (abs(getTile(x_pos - 1, y_pos)->getDistance() - currentCellDist) > biggestDifference) {
                            biggestDifference = abs(getTile(x_pos - 1, y_pos)->getDistance());
                            cellToBreak = WEST;
                        }
                    }

                    if (biggestDifference > 0) { // TODO: figure out why '>5' still generates solo pegs
                        switch (cellToBreak) { // Break wall across greates gradiant
                        case NORTH:
                            setWall(x_pos, y_pos, NORTH, false);
                            break;
                        case SOUTH:
                            setWall(x_pos, y_pos, SOUTH, false);
                            break;
                        case EAST:
                            setWall(x_pos, y_pos, EAST, false);
                            break;
                        case WEST:
                            setWall(x_pos, y_pos, WEST, false);
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
            if (rand() <= dirThreshold) {  // if random variable is below threshold move in last direction
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

        switch (direction) {
        case NORTH:
                setWall(x_pos, y_pos, NORTH, false);
                getTile(x_pos, y_pos + 1)->setExplored(true);
                getTile(x_pos, y_pos + 1)->setDistance(toExplore.size()); // Size of stack is distance from center
                toExplore.push(getTile(x_pos, y_pos + 1)); // Push next cell unto stack
                exploreDIR = NORTH;
                break;
            case EAST:
                setWall(x_pos, y_pos, EAST, false);
                getTile(x_pos + 1, y_pos)->setExplored(true);
                getTile(x_pos + 1, y_pos)->setDistance(toExplore.size());
                toExplore.push(getTile(x_pos + 1, y_pos));
                exploreDIR = EAST;
                break;
            case SOUTH:
                setWall(x_pos, y_pos, SOUTH, false);
                getTile(x_pos, y_pos - 1)->setExplored(true);
                getTile(x_pos, y_pos - 1)->setDistance(toExplore.size());
                toExplore.push(getTile(x_pos, y_pos - 1));
                exploreDIR = SOUTH;
                break;
            case WEST:
                setWall(x_pos, y_pos, WEST, false);
                getTile(x_pos - 1, y_pos)->setExplored(true);
                getTile(x_pos - 1, y_pos)->setDistance(toExplore.size());
                toExplore.push(getTile(x_pos - 1, y_pos));
                exploreDIR = WEST;
                break;
            }
    }

    breakGradientWall(); // Break one wall down with biggest gradient across it

    // Set each tile as unexplored and distance 255 by, the Mouse
    // (note that we just "borrowed" these fields for the maze genereration.
    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {
            getTile(x, y)->setDistance(255);
            getTile(x, y)->setExplored(false);
        }
    }

    addMinPeg(); // Make sure every peg has a wall around it *Compliance*

    hollowCenter(); // Hollow out center
    surroundCenter(); // Make sure the center is completely surrounded
    pathIntoCenter(); // Now break down one of those wall at random

    // Once the walls are assigned, we can assign neighbors
    assignNeighbors();
    */
}

/*
void Maze::initializeMaze() {

    int width = getWidth();
    int height = getHeight();

    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {
            for (Direction direction : DIRECTIONS) {
                // Set all walls to true as algo will break down walls // TODO
                getTile(x, y)->setWall(direction, true);
            }
        }
    }
}
*/

void Maze::hollowCenter() {
    // Ensures that the middle is hallowed out

    int width = getWidth();
    int height = getHeight();

    if (width % 2 == 0) {
        if (height % 2 == 0) {
            setWall(width / 2 - 1, height / 2, SOUTH, false);
            setWall(width / 2, height / 2, SOUTH, false);
            setWall(width / 2, height / 2 - 1, WEST, false);
        }
        setWall(width / 2, height / 2, WEST, false);

    }
    if (height % 2 == 0) {
        setWall(width / 2, height / 2, SOUTH, false);
    }
}
void Maze::pathIntoCenter() {

    int width = getWidth();
    int height = getHeight();

    if (width % 2 == 0 && height % 2 != 0) {
        switch (rand() / (1 + RAND_MAX / 6)) {
        case 0:
            setWall(width / 2 - 1, height / 2, WEST, false);
            break;
        case 1:
            setWall(width / 2 - 1, height / 2, NORTH, false);
            break;
        case 2:
            setWall(width / 2 - 1, height / 2, SOUTH, false);
            break;
        case 3:
            setWall(width / 2, height / 2, EAST, false);
            break;
        case 4:
            setWall(width / 2, height / 2, NORTH, false);
            break;
        case 5:
            setWall(width / 2, height / 2, SOUTH, false);
            break;
        }
    }

    // Break a random wall leading to center group
    if (height % 2 == 0 && width % 2 != 0) {
        switch (rand() / (1 + RAND_MAX / 6)) {
        case 0:
            setWall(width / 2, height / 2 - 1, SOUTH, false);
            break;
        case 1:
            setWall(width / 2, height / 2 - 1, EAST, false);
            break;
        case 2:
            setWall(width / 2, height / 2 - 1, WEST, false);
            break;
        case 3:
            setWall(width / 2, height / 2, NORTH, false);
            break;
        case 4:
            setWall(width / 2, height / 2, EAST, false);
            break;
        case 5:
            setWall(width / 2, height / 2, WEST, false);
            break;
        }
    }

    // Break a random wall leading to center group
    if (height % 2 == 0 && width % 2 == 0) {
        switch (rand() / (1 + RAND_MAX / 8)) {
        case 0:
            setWall(width / 2 - 1, height / 2 - 1, WEST, false);
            break;
        case 1:
            setWall(width / 2 - 1, height / 2 - 1, SOUTH, false);
            break;
        case 2:
            setWall(width / 2 - 1, height / 2, NORTH, false);
            break;
        case 3:
            setWall(width / 2 - 1, height / 2, WEST, false);
            break;
        case 4:
            setWall(width / 2, height / 2 - 1, EAST, false);
            break;
        case 5:
            setWall(width / 2, height / 2 - 1, SOUTH, false);
            break;
        case 6:
            setWall(width / 2, height / 2, NORTH, false);
            break;
        case 7:
            setWall(width / 2, height / 2, EAST, false);
            break;
        }
    }
}

void Maze::surroundCenter() {

    int width = getWidth();
    int height = getHeight();

    if (width % 2 == 0 && height % 2 != 0) {

        setWall(width / 2 - 1, height / 2, WEST, true);
        setWall(width / 2 - 1, height / 2, NORTH, true);
        setWall(width / 2 - 1, height / 2, SOUTH, true);

        setWall(width / 2, height / 2, EAST, true);
        setWall(width / 2, height / 2, NORTH, true);
        setWall(width / 2, height / 2, SOUTH, true);
    }

    // Break a random wall leading to center group
    if (height % 2 == 0 && width % 2 != 0) {

        setWall(width / 2, height / 2 - 1, SOUTH, true);
        setWall(width / 2, height / 2 - 1, EAST, true);
        setWall(width / 2, height / 2 - 1, WEST, true);

        setWall(width / 2, height / 2, NORTH, true);
        setWall(width / 2, height / 2, EAST, true);
        setWall(width / 2, height / 2, WEST, true);
    }

    // Break a random wall leading to center group
    if (height % 2 == 0 && width % 2 == 0) {

        setWall(width / 2 - 1, height / 2 - 1, WEST, true);
        setWall(width / 2 - 1, height / 2 - 1, SOUTH, true);

        setWall(width / 2 - 1, height / 2, NORTH, true);
        setWall(width / 2 - 1, height / 2, WEST, true);

        setWall(width / 2, height / 2 - 1, EAST, true);
        setWall(width / 2, height / 2 - 1, SOUTH, true);

        setWall(width / 2, height / 2, NORTH, true);
        setWall(width / 2, height / 2, EAST, true);
    }
}
void Maze::breakGradientWall() {
    //Break one wall with the biggest gradiant across it down

    /*
    int width = getWidth();
    int height = getHeight();

    int biggestDifference = 0;
    int x_pos = 0; // x and y position at which the biggest diference occured
    int y_pos = 0;

    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {

            int currentCellDist = getTile(x, y)->getDistance();
            int prospectiveDist = 0;

            // If cell to the North is valid
            if ((y + 1 < getHeight())) {
                prospectiveDist = getTile(x, y + 1)->getDistance();
                //Check the gradient and see if it is the biggest one so far
                if (abs(prospectiveDist - currentCellDist) > biggestDifference) {
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x; //Record x,y position to return later
                    y_pos = y;
                }
            }
            // If cell to the East is valid
            if ((x + 1 < getWidth())) {
                prospectiveDist = getTile(x + 1, y)->getDistance();
                if (abs(prospectiveDist - currentCellDist) > biggestDifference) {
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x;
                    y_pos = y;
                }
            }
            // If cell to the South is valid
            if ((y - 1 >= 0)) {
                prospectiveDist = getTile(x, y - 1)->getDistance();
                if (abs(prospectiveDist - currentCellDist) > biggestDifference) {
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x;
                    y_pos = y;
                }
            }
            // If cell to the West is valid
            if ((x - 1 >= 0)) {
                prospectiveDist = getTile(x - 1, y)->getDistance();
                if (abs(prospectiveDist - currentCellDist) > biggestDifference) {
                    biggestDifference = abs(prospectiveDist - currentCellDist);
                    x_pos = x;
                    y_pos = y;
                }
            }
        }
    }

    int currentCellDist = getTile(x_pos, y_pos)->getDistance();

    // Break down wall of cell which is between the gradient of size 'biggestDifference'

    if ((y_pos + 1 < getHeight()) && abs(getTile(x_pos, y_pos + 1)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference)
        setWall(x_pos, y_pos, NORTH, false);

    if ((x_pos + 1 < getWidth()) && abs(getTile(x_pos + 1, y_pos)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference)
        setWall(x_pos, y_pos, EAST, false);

    if ((y_pos - 1 >= 0) && abs(getTile(x_pos, y_pos - 1)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference)
        setWall(x_pos, y_pos, SOUTH, false);

    if ((x_pos - 1 >= 0) && abs(getTile(x_pos - 1, y_pos)->getDistance() - getTile(x_pos, y_pos)->getDistance()) == biggestDifference)
        setWall(x_pos, y_pos, WEST, false);
    */
}

void Maze::addMinPeg() {
    // Make sure every peg has at least one wall

    int width = getWidth();
    int height = getHeight();

    for (int x = 0; x < width - 1; x += 1) {
        for (int y = 0; y < height - 1; y += 1) {
            if (!getTile(x, y)->isWall(NORTH)
                && !getTile(x, y)->isWall(EAST)
                && !getTile(x + 1, y + 1)->isWall(WEST)
                && !getTile(x + 1, y + 1)->isWall(SOUTH)) {
                switch (rand() / (1 + RAND_MAX / 4)) {
                case NORTH:
                    setWall(x + 1, y + 1, WEST, true);
                    break;
                case EAST:
                    setWall(x + 1, y + 1, SOUTH, true);
                    break;
                case SOUTH:
                    setWall(x, y, EAST, true);
                    break;
                case WEST:
                    setWall(x, y, NORTH, true);
                    break;
                }
            }
        }
    }
}

void Maze::setWall(int x, int y, Direction direction, bool value) {

    int deltaX = 0;
    int deltaY = 0;
    Direction oppositeDir;

    switch (direction) {
        case NORTH:
            deltaY = 1;
            oppositeDir = SOUTH;
            break;
        case SOUTH:
            deltaY = -1;
            oppositeDir = NORTH;
            break;
        case EAST:
            deltaX = 1;
            oppositeDir = WEST;
            break;
        case WEST:
            deltaX = -1;
            oppositeDir = EAST;
            break;
    }

    getTile(x, y)->setWall(direction, value);
    getTile(x + deltaX, y + deltaY)->setWall(oppositeDir, value);
}
#endif

} // namespace sim
