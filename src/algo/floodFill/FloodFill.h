#ifndef FLOODFILL_H_
#define FLOODFILL_H_

#include <list>

#include "../IAlgorithm.h"
#include "Cell.h"
#include "History.h"

// Constants
static const int MAZE_SIZE = 16; // TODO: Be able to set maze width and height separately
static const int SHORT_TERM_MEM = 5; // Steps that are forgetten by the mouse after an error
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};


class FloodFill : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse); // IAlgorithm interface method

private:
    sim::MouseInterface* m_mouse; // Mouse used to traverse and solve the maze
    Cell m_cells[MAZE_SIZE][MAZE_SIZE]; // Grid a cells to store maze information
    int m_x; // X position of the mouse
    int m_y; // Y position of the mouse
    int m_d; // Direction of the mouse
    int m_steps; // Count of the steps of the mouse

    void printDistances(); // Prints the distance values of the cells in the maze
    void printWalls(); // Prints the wall values of the cells in the maze
    void resetColors(); // Resets the colors to the maze through the MouseInterface

    void initialize(); // Initialize all mutable fields, including the Cells' walls
                       // and distances, as well as the mouse-related fields. This 
                       // should only be called when the mouse is actually in the
                       // starting location.

    void victory(); // Once the maze is fully explored, solve as quick as possible

    void walls(); // Updates the walls surrounding the robot
    void flood(int x, int y); // Floods the maze corresponding to new walls
    void moveTowardsGoal(); // Moves the mouse one step towards the goal (lower distance value)
    bool inGoal(int x, int y); // Returns true if the cell at (x, y) is in the center

    void moveForward(); // Updates x and y and moves mouse
    void turnRight(); // Updates direction and turns mouse
    void turnLeft(); // Updates direction and turns mouse

    Cell* getFrontCell(); // Returns the cell in front of the mouse
    Cell* getLeftCell(); // Returns the cell to the right of the mouse
    Cell* getRightCell(); // Returns the cell to the left of the mouse
    Cell* getRearCell(); // Returns the cell in the rear of the mouse
    bool spaceFront(); // Returns true if there's a cell in front of the mouse
    bool spaceLeft(); // Returns true if there's a cell to the left of the mouse
    bool spaceRight(); // Returns true if there's a cell to the right of the mouse
    int min(int one, int two, int three, int four); // Returns the min of four ints

    // Basic Floodfill Algorithm
    void basicFloodFill();


    // ------------------ Explore Algorithm Utilities ----------------------- //

    // Explores the entire searchable maze and fills the walls and distances,
    // eventually returning the mouse back to the starting location
    void explore();

    void exploreBeta(); // USED FOR TESTING PURPOSES ONLY //

    // Returns whether or not the location is one cell away
    bool isOneCellAway(Cell* target);

    // Moves the mouse to the target Cell but only if it's exactly one Cell away
    void moveOneCell(Cell* target);

    // Attempts to short-circuit the retracing by looking at neighbors of untraversed cells
    // Returns true if successful, false if short-circuiting wasn't possible
    bool tryUntraversed(Cell* target);


    // ----------------------- Request Utilities --------------------------- //

    History m_history; // History object used for undos
    bool m_explored; // Whether or not the explore method was has completed
    bool m_checkpointReached; // Whether or not we've made it back to the checkpoint

    // Updates a current cell along the path to return to checkpoint after an undo request
    void cellUpdate(); 

    // Moves to the checkpoint and updates cell values along the way. Returns true if either
    // a reset or undo request was made during the movement, false otherwise
    bool proceedToCheckpoint(std::stack<Cell*> path);

    // Checks and handles requests for the explore method, returns true for reset, else false
    bool checkRequestExplore();

    // Checks and handles requests for the victory method, returns true for reset, else false
    bool checkRequestVictory();
};

#endif // FLOODFILL_H_
