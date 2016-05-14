#pragma once

#include "../IMazeAlgorithm.h"

#include <vector>
#include <stack>

namespace tomasz {

enum Direction { NORTH, EAST, SOUTH, WEST, UNDEFINED };

// A class that generates a realistic maze at random
class TomaszMazeGenerator : public IMazeAlgorithm {

public:

    void generate(int mazeWidth, int mazeHeight, sim::MazeInterface* maze);

    // TODO: probably rename this...
    void convertToBasicMaze(sim::MazeInterface* maze);
    
private:

    // Tomasz Maze Generator Parameters
    // Number between 0-1 which determines how straight the maze becomes closer to edges
    double tomStraightFactor = 0.85;
    // Number between 0-1 which determines if the algorithm will break down a wall at a dead end
    double tomDeadEndBreakChance = 0.75;
    // The minimum distance between cells to break down a dead end wall. > 5
    int tomDeadEndBreakThreshold = 8;
    // Number of walls to break down in finalized maze
    int tomGradientWallBreaks = 3;

    // Struct which is used internally to keep a representation of the maze    
    struct TomMazeGenTile {
    
        // X position of Tile
    	int row;
        
        // Y postion of Tile
	    int column;
        
        // The distance the tile is from the start of the maze
	    int distanceFromStart;
        
        // whether or not the cell was processed by the algorithm
	    bool explored;
        
        // whether the tile is located in the middle or not
	    bool isCenter;
        
        // a map of the wall values.  Keys are NORTH, SOUTH, EAST, WEST.
        // False for no wall, true for the presence of a wall
    	std::map<Direction, bool> walls;
    };

    // generate a maze, but do not return it
    void generateMaze(int mazeWidth, int mazeHeight);
    
    // Method will break a wall from cell x,y in the direction of the biggest gradient
    void breakGradientWall(int xPos, int yPos);
    
    // Moves from position xPos, yPos, toward Direction directionToMove
    // Adds the tile to the stack (m_stack)
    Direction moveTowardDirection(int xPos, int yPos, Direction direction);
    
    // Run a breadth first search through the maze and update the distance
    // from start values of tiles.
    // If an x and y position are supplied, distances from that cell are computed
    void updateDistanceFromStart(int xPos = 0, int yPos = 0);
    
    // Determines which way the algorithm should procede.  This takes into account the
    // configuration values and determine whether or not the algorithm should turn or not
    Direction getDirectionToMove(float moveConst, std::map<Direction, bool> choices);
    
    //Initialize a maze with walls in every location.  Mark center as exlpored
    // and set the special center flag so the maze generation algorithm will not
    // go in.  All methods with potention to make walls in center will check this
    // flag.  This way, we do not have to hollow out the center later.
    void initializeMaze();
    
    // Set a wall a location and direction to a particulare state.
    // Note: this method takes care of the wall in the other cell
    // ------- ------
    // |     | |  
    // | --->| |<---
    // |     | |
    // ------- ------
    void setWall(int x, int y, Direction direction, bool value);
    
    // Checks if cell x, y is within the maze boundries, is unexplored, and is not the center
    // If a direction is supplied, the method will check the cell to the NORTH, EAST, SOUTH, or
    // WEST of cell x, y
    bool isValidUnexploredMove(int x, int y, Direction direction = UNDEFINED);
    
    // Checks if cell x, y is within the maze boundries, is explored, and is not the center
    // If a direction is supplied, the method will check the cell to the NORTH, EAST, SOUTH, or     
    // WEST of cell x, y
    bool isValidExploredTile(int x, int y, Direction direction = UNDEFINED);
    
    // Breaks a number of walls which are across the biggest gradient
    // A gradient is defined as the difference in distance_from_start
    // in two side by side cells
    void breakGradientWalls();
       
    // Helper to the formCenter above.  Hollows out the center, and sets the tiles as center
    void makeCenter();
    
    // Finds the longest distance from beginning
    // that is touching the center of tiles, and breaks that wall.
    void pathIntoCenter();
    
    // Get a pointer to the tile at location x and y from the vector of vectors
    // If the direction is specified, it will get the tile in that direction from
    // tile x, y
    // If a direction is supplied, the method will return the cell to the NORTH, EAST, SOUTH, or
    // WEST of cell x, y
    TomMazeGenTile* getTile(int x, int y, Direction direction = UNDEFINED);
    
    // Vector of vectors which is the representation of the maze
    std::vector<std::vector<TomMazeGenTile>> m_maze;
    
    // Stack for the depth first search
    std::stack<TomMazeGenTile*> m_stack;
    
    // Width of the maze
    int m_width;
    
    // Height of the maze
    int m_height;
    
    // The current direction of the path being generated
    Direction m_direction;

    // Interface for the maze algorithm
    sim::MazeInterface* m_mazeInterface;

};

} // namespace tomasz
