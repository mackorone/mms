#include "MackAlgo.h"

// TODO: Remove these
#include <iostream>

// XXX: Don't use STL

namespace mack {

void MackAlgo::solve(sim::MouseInterface* mouse) {

    // Bookkeeping
    m_mouse = mouse;
    m_mouse->declareInterfaceType(sim::DISCRETE);

    // First, set the positions of all 
    /*
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            m_maze[x][y].setPosition(x, y);
        }
    }
    */

    // Set the wall values for the cells
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            m_maze[x][y].setWall(NORTH, (y == MAZE_HEIGHT - 1 ? true : false));
            m_maze[x][y].setWall(EAST, (x == MAZE_WIDTH - 1 ? true : false));
            m_maze[x][y].setWall(SOUTH, (y == 0 ? true : false));
            m_maze[x][y].setWall(WEST, (x == 0 ? true : false));
        }
    }

    /*
    for (int y = 0; y < MAZE_HEIGHT/2; y += 1) { 
        int distance = MAZE_WIDTH/2 + MAZE_HEIGHT/2 - 2 - y; 
        for(int x = 0; x < MAZE_WIDTH/2; x += 1) { 
            m_maze[x][y].setDistance(distance);
            m_maze[x][MAZE_HEIGHT-1-y].setDistance(distance);
            m_maze[MAZE_WIDTH-1-x][MAZE_HEIGHT-1-y].setDistance(distance);
            m_maze[MAZE_WIDTH-1-x][y].setDistance(distance);
            distance -= 1;
        }    
    }    

    // Initialize other values for the Cells
    for (int y = 0; y < MAZE_HEIGHT; y += 1){
        for (int x = 0; x < MAZE_WIDTH; x += 1){

            // Set the walls values
            m_maze[x][y].setWall(SOUTH, y == 0);
            m_maze[x][y].setWall(NORTH, y == MAZE_HEIGHT-1);
            m_maze[x][y].setWall(WEST, x == 0);
            m_maze[x][y].setWall(EAST, x == MAZE_WIDTH-1);

            // Declare the walls
            m_mouse->declareWall(x, y, directionToChar(SOUTH), y == 0);
            m_mouse->declareWall(x, y, directionToChar(NORTH), y == MAZE_HEIGHT-1);
            m_mouse->declareWall(x, y, directionToChar(WEST), x == 0);
            m_mouse->declareWall(x, y, directionToChar(EAST), x == MAZE_WIDTH-1);

            // Set the inspected values of the walls
            m_maze[x][y].setWallInspected(SOUTH, y == 0);
            m_maze[x][y].setWallInspected(NORTH, y == MAZE_HEIGHT-1);
            m_maze[x][y].setWallInspected(WEST, x == 0);
            m_maze[x][y].setWallInspected(EAST, x == MAZE_WIDTH-1);

            // Set the prev and explored values of the Cells
            m_maze[x][y].setPrev(NULL);
            m_maze[x][y].setExplored(false);
            m_maze[x][y].setTraversed(false);
        }    
    */

    // Assign the neighbors of every cell
    // TODO: At the beginning of the maze, a good heuristic is to move towards the center if you can
    // BFS from every position, move towards center if possible
}

/*
void MackAlgo::
*/

void MackAlgo::setColor(int x, int y, char color) {
    m_mouse->colorTile(x, y, color);
}

void MackAlgo::resetColors() {
    for (int x = 0; x < MAZE_WIDTH; x += 1) {
        for (int y = 0; y < MAZE_HEIGHT; y += 1) {
            setColor(x, y, 'k');
        }
    }
}


} // namespace mack
