#pragma once

#include <list>

#include "../IMouseAlgorithm.h"
#include "Cell.h"
#include "Cellmod.h"
#include "History.h"
#include "SimpleCellmod.h"

namespace floodFill {

// Constants
static const int MAZE_SIZE_X = 16; // Length of X axis of maze
static const int MAZE_SIZE_Y = 16; // Length of Y axis of maze
static const int SHORT_TERM_MEM = 8; // Steps that are forgetten by the mouse after an error
static const bool ALGO_COMPARE = false; // Whether or not we're comparing the solving algorithms
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

class FloodFill : public IMouseAlgorithm {

public:
    bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    sim::MouseInterface* m_mouse; // A pointer to the mouse interface
    Cell m_cells[MAZE_SIZE_X][MAZE_SIZE_Y]; // Grid a cells to store maze information
    int m_x; // X position of the mouse
    int m_y; // Y position of the mouse
    int m_d; // Direction of the mouse
    int m_steps; // Count of the steps of the mouse
    bool m_centerReached; // Whether or not the mouse has reached the center at least once
    bool m_explored; // Whether or not the explore method was has completed
    History m_history; // History object used for undos
    bool m_checkpointReached; // Whether or not we've made it back to the checkpoint

    void justFloodFill(); // Vanilla Floodfill algo
    void simpleSolve(); // Solves using basic floodfill (not perfect)
    void extensiveSolve(); // Solves using explore (guaranteed perfect)

    void printDistances(); // Prints the distance values of the cells in the maze
    void printWalls(); // Prints the wall values of the cells in the maze
    void resetColors(); // Resets the colors to the maze through the MouseInterface

    void initialize(); // Initialize all mutable fields, including the Cells' walls
                       // and distances, as well as the mouse-related fields. This 
                       // should only be called when the mouse is actually in the
                       // starting location.

    void victory(); // Once the maze is fully explored, solve as quick as possible

    void walls(); // Updates the walls surrounding the robot
    void checkDeadEnd(Cell* cell); // Deduces fourth wall value, if possible
    void flood(int x, int y); // Floods the maze corresponding to new walls
    void moveTowardsGoal(); // Moves the mouse one step towards the goal (lower distance value)
    bool inGoal(int x, int y); // Returns true if the cell at (x, y) is in the center

    void moveForward(); // Updates x and y and moves mouse
    void turnRight(); // Updates direction and turns mouse
    void turnLeft(); // Updates direction and turns mouse
    void turnAround(); // Updates direction and turns mouse

    Cell* getFrontCell(); // Returns the cell in front of the mouse
    Cell* getLeftCell(); // Returns the cell to the right of the mouse
    Cell* getRightCell(); // Returns the cell to the left of the mouse
    Cell* getRearCell(); // Returns the cell in the rear of the mouse
    bool spaceFront(); // Returns true if there's a cell in front of the mouse
    bool spaceLeft(); // Returns true if there's a cell to the left of the mouse
    bool spaceRight(); // Returns true if there's a cell to the right of the mouse
    int min(int one, int two, int three, int four); // Returns the min of four ints
    char directionToChar(int direction); // Converts 0123 to nesw

    bool undoRequested();
    bool resetRequested();
    void undoHonored();
    void resetHonored();

    // Moves the mouse to the target Cell, but only if it's exactly one Cell away
    void moveOneCell(Cell* target);

    // ------------------- Basic FloodFill Utilities ----------------------- //

    // Basic Floodfill Algorithm
    void bffExplore(std::stack<Cell*>* path);

    // Performs the updates for cells in the basic floodfill algo
    void dobffCellUpdates();

    // Appends a newly modified cell to the modified cells list for each step in the algo
    void bffAppendModifiedCell(std::list<SimpleCellmod>* modCellsList, Cell* cell);

    // Repeatedly solves the maze using the path found by bffExplore
    void bffVictory(std::stack<Cell*> path);


    // ------------------ Explore Algorithm Utilities ----------------------- //

    // Explores the entire searchable maze and fills the walls and distances,
    // eventually returning the mouse back to the starting location
    void explore();

    // Updates all values for the Cell that the mouse is currently in. Additionally
    // updates the history and the current stack of unexplored Cells
    void doUpdatesForCurrentCell(std::stack<Cell*>* unexplored);

    // Returns whether or not the location is one cell away
    bool isOneCellAway(Cell* target);

    // Attempts to short-circuit the retracing by looking at neighbors of untraversed
    // cells. Returns true if successful, false if short-circuiting wasn't possible.
    bool tryUntraversed(Cell* target);


    // -------------------- Explore Request Utilities ----------------------- //

    // Appends a newly modified cell to the modified cells list for each step in the algo
    void appendModifiedCell(std::list<Cellmod>* modList, Cell* cell);

    // Moves to the checkpoint and updates cell values along the way. Returns true if either
    // a reset or undo request was made during the movement, false otherwise
    bool proceedToCheckpoint(std::stack<Cell*> path);

    // Checks and handles requests for the victory method, returns true for reset, else false
    bool checkRequestVictory();
};

} // namespace floodFill
