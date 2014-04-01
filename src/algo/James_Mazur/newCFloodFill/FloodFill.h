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
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

// These method calls are implemented by hardware - in other words, these 6 calls form an interface to the hardware that the algorithm
// code can use easily.

// m_mouse...
bool _wallFront(void);
bool _wallRight(void);
bool _wallLeft(void);
void _moveForward(void); // Added underscores to avoid namespace collision - like syscalls!
void _turnRight(void);
void _turnLeft(void);
void _turnAround(void);

// These ones are new!!
bool _resetRequested(void);
void _resetHonored(void);
bool _undoRequested(void);
void _undoHonored(void);
void _resetPosition(void); // Spin until the mouse is brought back to the center

// Global variables.
struct Cell m_cells[MAZE_SIZE_X][MAZE_SIZE_Y]; // Grid of cells to store maze information
int m_x; // X position of the mouse
int m_y; // Y position of the mouse
int m_d; // Direction of the mouse
int m_steps; // Count of the steps of the mouse
bool m_centerReached; // Whether or not the mouse has reached the center at least once
bool m_explored; // whether or not the explore method has completed
struct History m_history; // struct History used for undos
bool m_checkpointReached; // whether or not we've made it back to the checkpoint

bool firstHInitialize;

// Methods used in "FloodFill.c".
void solve(void);


void justFloodFill(void); // Vanilla Floodfill algo
void simpleSolve(void); // not guaranteed perfect
void extensiveSolve(void); // guaranteed perfect

void printDistances(void); // Prints the distance values of the cells in the maze
void printWalls(void); // Prints the wall values of the cells in the maze

void initialize(void); // Initializes Cells' wall and distance values

void victory(void); // Once the maze is fully explored, solve as quickly as possible

void walls(void); // Updates the walls surrounding the robot
void checkDeadEnd(struct Cell *); // deduces fourth wall value, if possible
void flood(int,int); // Floods the maze corresponding to new walls
void moveTowardsGoal(void); // Moves the mouse one step towards the goal (lower distance value)
bool inGoal(int x, int y); // Returns true if the cell at (x,y) is in the center

void moveForward(void); // Updates x and y and moves mouse
void turnRight(void); // Updates direction and turns mouse
void turnLeft(void); // Updates direction and turns mouse
void turnAround(void); // Updates direction and turns mouse

struct Cell * getFrontCell(void);
struct Cell * getLeftCell(void);
struct Cell * getRightCell(void);
struct Cell * getRearCell(void);
bool spaceFront(void);
bool spaceLeft(void);
bool spaceRight(void);
int min(int,int,int,int);

void moveOneCell(struct Cell *);

// bff functions
void bffExplore(struct CellStack *);
void dobffCellUpdates(void);
void bffAppendModifiedCell(struct List *, struct Cell *);
void bffVictory(struct CellStack *);

// Floodfill
void explore(void);
void doUpdatesForCurrentCell(struct CellStack *);
bool isOneCellAway(struct Cell *);
bool tryUntraversed(struct Cell *);

// History
void appendModifiedCell(struct List *, struct Cell *);
bool proceedToCheckpoint(struct CellStack *);
bool checkRequestVictory(void);

#endif // FLOODFILL_H_
