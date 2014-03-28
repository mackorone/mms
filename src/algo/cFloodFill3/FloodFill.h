#ifndef FLOODFILL_H_
#define FLOODFILL_H_

#include "Cell.h"
#include "Cellmod.h"
#include "CellStack.h"
#include "History.h"
#include "SimpleCellmod.h"

#include <stdbool.h>

// Constants.
#define MAZE_SIZE_X 16
#define MAZE_SIZE_Y 16
#define SHORT_TERM_MEM 12
#define ALGO_COMPARE true

// These method calls are implemented by hardware - in other words, these 6 calls form an interface to the hardware that the algorithm
// code can use easily.
bool _wallFront(void);
bool _wallRight(void);
bool _wallLeft(void);
void _moveForward(void); // Added underscores to avoid namespace collision - like syscalls!
void _turnRight(void);
void _turnLeft(void);

// These ones are new!!
bool _resetRequested(void);
void _resetHonored(void);
bool _undoRequested(void);
void _undoHonored(void);

// Global variables.
struct Cell m_cells[MAZE_SIZE_X][MAZE_SIZE_Y]; // Grid of cells to store maze information
int m_x; // X position of the mouse
int m_y; // Y position of the mouse
int m_d; // Direction of the mouse
int m_steps; // Count of the steps of the mouse

bool m_centerReached; // Whether or not the mouse has reached the center at least once

struct History m_history; // struct History used for undos
bool m_explored; // whether or not the explore method has completed
bool m_checkpointReached; // whether or not we've made it back to the checkpoint

bool m_bffDone;
struct Cell *m_bffCp;

bool firstHInitialize;

enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

// Methods used in "FloodFill.c".
void solve(void);

void checkDeadEnd(struct Cell *); // deduces fourth wall value, if possible

void simpleSolve(void); // not guaranteed perfect
void extensiveSolve(void); // guaranteed perfect
void justFloodFill(void); // Vanilla Floodfill algo

void printDistances(void); // Prints the distance values of the cells in the maze
void printWalls(void); // Prints the wall values of the cells in the maze
void initialize(void); // Initializes Cells' wall and distance values

void walls(void); // Updates the walls surrounding the robot
void flood(int,int); // Floods the maze corresponding to new walls
void moveTowardsGoal(void); // Moves the mouse one step towards the goal (lower distance value)

void victory(void); // Once the maze is fully explored, solve as quickly as possible

void moveForward(void); // Updates x and y and moves mouse
void turnRight(void); // Updates direction and turns mouse
void turnLeft(void); // Updates direction and turns mouse
void turnAround(void); // Updates direction and turns mouse

bool inGoal(int x, int y); // Returns true if the cell at (x,y) is in the center

int min(int,int,int,int);

struct Cell * getFrontCell(void);
struct Cell * getLeftCell(void);
struct Cell * getRightCell(void);
struct Cell * getRearCell(void);

bool spaceFront(void);
bool spaceLeft(void);
bool spaceRight(void);

void explore(void);
void doUpdatesForCurrentCell(struct CellStack *);

void moveOneCell(struct Cell *);
bool isOneCellAway(struct Cell *);
bool tryUntraversed(struct Cell *);

void bffExplore(struct CellStack *);
void dobffCellUpdates(void);
void bffVictory(struct CellStack *);

// Appends a newly modified cell to the modified cells list for each step in the algo
void bffAppendModifiedCell(std::list<SimpleCellmod> *, struct Cell *);

// Appends a newly modified cell to the modified cells list for each step in the algo
void appendModifiedCell(std::list<Cellmod> *, struct Cell *);

// Moves to the checkpoint and updates cell values along the way. Returns true if either
// a reset or undo request was made during the movement, false otherwise
bool proceedToCheckpoint(struct CellStack *);

// Checks and handles requests for the victory method, returns true for reset, else false
bool checkRequestVictory(void);

#endif // FLOODFILL_H_
