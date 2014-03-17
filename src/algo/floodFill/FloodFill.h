#ifndef FLOODFILL_H_
#define FLOODFILL_H_

#include "../IAlgorithm.h"
#include "Cell.h"

// Constants
static const int MAZE_SIZE = 16;
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
    void initialize(); // Initializes Cells' wall and distance values

    void walls(); // Updates the walls surrounding the robot
    void flood(int x, int y); // Floods the maze corresponding to new walls
    void moveTowardsGoal(); // Moves the mouse one step towards the goal (lower distance value)
    bool inGoal(); // Returns true if the mouse is in the center

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

    bool untraversedRetraceLeft();

    // ---------------------- Unnecessary Utilities -------------------------- //

    // Can be used to set the destination to any cell
    void initializeDestinationTile(int x, int y);
};

#endif // FLOODFILL_H_
