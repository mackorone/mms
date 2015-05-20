#include "MazeChecker.h"

#include "Assert.h"
#include "SimUtilities.h"

namespace sim {

bool MazeChecker::validMaze(const std::vector<std::vector<BasicTile>>& maze) {

    // TODO: Should it be "rectangular" instead of "square?"

    if (!isSquare(maze)) {
        SimUtilities::print("Error: The maze is not square.");
        return false;
    }

    if (!isEnclosed(maze)) {
        SimUtilities::print("Error: The maze is not enclosed by walls.");
        return false;
    }

    if (!hasConsistentWalls(maze)) {
        SimUtilities::print("Error: The maze does not have consistent walls.");
        return false;
    }

    return true;
}

bool MazeChecker::isSquare(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: SOM - implement this method
    return true;
}

bool MazeChecker::isEnclosed(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: SOM - implement this method
    return true;
}

bool MazeChecker::hasConsistentWalls(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: SOM - implement this method
    return true;
}

bool MazeChecker::officialMaze(const std::vector<std::vector<BasicTile>>& maze) {

    ASSERT(validMaze(maze));

    // TODO: SOM - The center should have no walls in it

    if (!hasPathToCenter(maze, 0, 0)) {
        SimUtilities::print("Error: The maze does not have a path to the center.");
        return false;
    }

    if (!hasOneEntrance(maze)) {
        SimUtilities::print("Error: The center of the maze has more than one entrance.");
        return false;
    }

    if (!eachPostHasWall(maze, 1, 1)) {
        SimUtilities::print("Error: There is at least one non-center post with no walls connected to it.");
        return false;
    }
    
    if (!threeStartingWalls(maze)) {
        SimUtilities::print("Error: There are not exactly three starting walls.");
        return false;
    }

    if (!unsolvableByWallFollower(maze)) {
        SimUtilities::print("Error: The maze is solvable by a maze-following robot.");
        return false;
    }

    if (!noInaccesibleLocations(maze)) {
        SimUtilities::print("Error: There are inaccessible locations in the maze.");
        return false;
    }

    return true;
}

/*
//Recursively check every combination and left and right wall follows
//to ensure the maze cannot be solved by a random wall follower
bool cannotWallFollow(const std::vector<std::vector<BasicTile>>& maze) {
    std::vector<std::vector<int>> orientations;

    //note: the 0,0,0 arguments assume the robot starts in coordinates
    //x=0, y=0, and facing North where cardinal directions NESW = 0123
    return !(recursiveLeftWallFollow(maze, 0, 0, 0, orientations) ||
             recursiveRightWallFollow(maze, 0, 0, 0, orientations));
}

bool recursiveLeftWallFollow(const std::vector<std::vector<BasicTile>>& maze, int x, int y, int dir, std::vector<std::vector<int>> orientations) {
    //check to see if center is found
    if((x==7||x==8)&&(y==7||y==8)){
        return true;
    }

    //Orientation contains information about prior positions of the solver
    //Each element is a vector containing:
    //[x coord, y coord, direction (NESW = 0123), and left (1) or right(2)]
    //The idea is that if the algorithm finds itself in an orientation
    //it's been in before without reaching the center, then it will
    //never reach the center
    std::vector<int> orientation;
    orientation.push_back(x);
    orientation.push_back(y);
    orientation.push_back(dir);
    orientation.push_back(1);
    bool dejavu = false;
    for(int i = 0; i < orientations.size(); i++) {
        for(int j = 0; j < orientation.size(); j++) {
            if(orientation.at(j) != orientations.at(i).at(j)) {
                break;
            }
            if(j == (orientation.size() - 1)) {
                dejavu = true;
            }
        }
        if(dejavu) {
            return false;
        }
    }
    //save current orientation for future comparison
    orientations.push_back(orientation);            
    
    //now move on like a normal left follower
    int left = dir - 1;
    if(left < 0) left = 3;

    int right = dir + 1;
    if(right > 3) right = 0;

    if(!maze.at(x).at(y).walls.at(left)){
        dir = left;
    }
    while(maze.at(x).at(y).walls.at(dir)){
        dir = right;
        right = dir + 1;
        if(right > 3) right = 0;
    }

    if(dir == 0) y++;
    else if(dir == 1) x++;
    else if(dir == 2) y--;
    else if(dir == 3) x--;

    //now branch into right and left followers from new position
    return recursiveLeftWallFollow(maze, x, y, dir, orientations) ||
           recursiveRightWallFollow(maze, x, y, dir, orientations);
}

//refer to recursiveLeftWallFollow for comments
bool recursiveRightWallFollow(const std::vector<std::vector<BasicTile>>& maze, int x, int y, int dir, std::vector<std::vector<int>> orientations) {
    if((x==7||x==8)&&(y==7||y==8)){
        return true;
    }

    std::vector<int> orientation;
    orientation.push_back(x);
    orientation.push_back(y);
    orientation.push_back(dir);
    orientation.push_back(2);
    bool dejavu = false;
    for(int i = 0; i < orientations.size(); i++) {
        for(int j = 0; j < orientation.size(); j++) {
            if(orientation.at(j) != orientations.at(i).at(j)) {
                break;
            }
            if(j == (orientation.size() - 1)) {
                dejavu = true;
            }
        }
        if(dejavu) {
            return false;
        }
    }
    orientations.push_back(orientation);            
    
    //now move on like a normal right follower
    int left = dir - 1;
    if(left < 0) left = 3;

    int right = dir + 1;
    if(right > 3) right = 0;

    if(!maze.at(x).at(y).walls.at(right)){
        dir = right;
    }
    while(maze.at(x).at(y).walls.at(dir)){
        dir = left;
        left = dir - 1;
        if(left < 0) left = 3;
    }

    if(dir == 0) y++;
    else if(dir == 1) x++;
    else if(dir == 2) y--;
    else if(dir == 3) x--;

    //now branch into right and left followers from new position
    return recursiveLeftWallFollow(maze, x, y, dir, orientations) ||
           recursiveRightWallFollow(maze, x, y, dir, orientations);
 
}
*/

bool MazeChecker::hasPathToCenter(const std::vector<std::vector<BasicTile>>& maze, int x, int y) {
    static std::vector<bool> checkRow(16, false);
    static std::vector<std::vector<bool>> checkTiles(16, checkRow);
    checkTiles.at(x).at(y) = true;
    bool eastCheck = false, southCheck = false, westCheck = false, northCheck = false;
    
    // Check for center tile
    if ((x==7||x==8)&&(y==7||y==8)) {
        return true;
    }

    // Check north tile
    if (y != 15 && !maze.at(x).at(y).walls.at(NORTH) && !checkTiles.at(x).at(y+1)) {
        northCheck = hasPathToCenter(maze, x, y+1);
    }

    // Check east tile
    if (x != 15 && !maze.at(x).at(y).walls.at(EAST) && !checkTiles.at(x+1).at(y)) {
        eastCheck = hasPathToCenter(maze, x+1, y);
    }
    // Check south tile
    if (y != 0 && !maze.at(x).at(y).walls.at(SOUTH) && !checkTiles.at(x).at(y-1)) { 
        southCheck = hasPathToCenter(maze, x, y-1);
    }
    // Check west tile
    if (x != 0 && !maze.at(x).at(y).walls.at(WEST) && !checkTiles.at(x-1).at(y)) {
        westCheck = hasPathToCenter(maze, x-1, y);
    }

    return eastCheck || southCheck || westCheck || northCheck;
}

bool MazeChecker::hasOneEntrance(const std::vector<std::vector<BasicTile>>& maze) {
    int entranceCounter = 0;
    
    //Check lower left entrances
    entranceCounter += !maze.at(7).at(7).walls.at(SOUTH);
    entranceCounter += !maze.at(7).at(7).walls.at(WEST);
    //Check upper left entrances
    entranceCounter += !maze.at(7).at(8).walls.at(NORTH);
    entranceCounter += !maze.at(7).at(8).walls.at(WEST);
    //Check lower right entrances
    entranceCounter += !maze.at(8).at(7).walls.at(SOUTH);
    entranceCounter += !maze.at(8).at(7).walls.at(EAST);
    //Check upper right entrances
    entranceCounter += !maze.at(8).at(8).walls.at(NORTH);
    entranceCounter += !maze.at(8).at(8).walls.at(EAST);

    return entranceCounter == 1;
}

bool MazeChecker::eachPostHasWall(const std::vector<std::vector<BasicTile>>& maze, int x, int y) {
    static std::vector<bool> checkRow(16, false);
    static std::vector<std::vector<bool>> checkTiles(16, checkRow);
    bool northCheck = true, eastCheck = true;    

    checkTiles.at(x).at(y) = true;
    
    //Check if upper right tile
    if (x == 15 && y == 15) {
        return true;
    }  

    int wallCount = 0;
    //Check bottom left post
    wallCount += maze.at(x).at(y).walls.at(WEST);
    wallCount += maze.at(x).at(y).walls.at(SOUTH);   
    if(x != 0) {
        wallCount += maze.at(x-1).at(y).walls.at(SOUTH);
    }
    if(y != 0) {
        wallCount += maze.at(x).at(y-1).walls.at(WEST);
    }

    if(wallCount == 0 && (x != 8 || y != 8)) {
        return false;
    }
    else {
        if(y != 15 && checkTiles.at(x).at(y+1) == false) {
            northCheck = eachPostHasWall(maze, x, y + 1);
        }
        if(x != 15 && checkTiles.at(x+1).at(y) == false) {
            eastCheck = eachPostHasWall(maze, x + 1, y);
        }
        return northCheck && eastCheck;
    }
}

bool MazeChecker::threeStartingWalls(const std::vector<std::vector<BasicTile>>& maze) {
    int wallCount = 0;
    wallCount += (maze.at(0).at(0).walls.at(NORTH) ? 1 : 0);
    wallCount += (maze.at(0).at(0).walls.at(EAST)  ? 1 : 0);
    wallCount += (maze.at(0).at(0).walls.at(SOUTH) ? 1 : 0);
    wallCount += (maze.at(0).at(0).walls.at(WEST)  ? 1 : 0);
    return wallCount == 3;
}

bool MazeChecker::unsolvableByWallFollower(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: SOM - implement this method
    return false;
}

bool MazeChecker::noInaccesibleLocations(const std::vector<std::vector<BasicTile>>& maze) {
    // TODO: SOM - implement this method
    return false;
}

} // namespace sim
