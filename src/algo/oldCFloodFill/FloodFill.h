#ifndef FLOODFILL_H_
#define FLOODFILL_H_

#include "Cell.h"

#include <stdbool.h>

// Constants for maze size.
#define X_WIDTH 16
#define Y_WIDTH 16

// These method calls are implemented by hardware - in other words, these 6 calls form an interface to the hardware that the algorithm
// code can use easily.
bool _wallFront(void);
bool _wallRight(void);
bool _wallLeft(void);
void _moveForward(void); // Added underscores to avoid namespace collision - like syscalls!
void _turnRight(void);
void _turnLeft(void);

// Global variables.
struct Cell m_cells[X_WIDTH][Y_WIDTH]; // Grid of cells to store maze information
int m_x; // X position of the mouse
int m_y; // Y position of the mouse
int m_d; // Direction of the mouse
int m_steps; // Count of the steps of the mouse

enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

// Methods used in "FloodFill.c".
void solve(void);

void printDistances(void); // Prints the distance values of the cells in the maze
void printWalls(void); // Prints the wall values of the cells in the maze
void initializeCells(void); // Initializes Cells' wall and distance values

void walls(void); // Updates the walls surrounding the robot
void flood(int,int); // Floods the maze corresponding to new walls
void moveTowardsGoal(void); // Moves the mouse one step towards the goal (lower distance value)

void moveForward(void); // Updates x and y and moves mouse
void turnRight(void); // Updates direction and turns mouse
void turnLeft(void); // Updates direction and turns mouse

bool inGoal(void); // Returns true if the mouse is in the center

int min(int,int,int,int);

struct Cell * getFrontCell(void);
struct Cell * getLeftCell(void);
struct Cell * getRightCell(void);
struct Cell * getRearCell(void);

bool spaceFront(void);
bool spaceLeft(void);
bool spaceRight(void);

void explore(void);

void moveOneCell(struct Cell *);
bool isOneCellAway(struct Cell *);

// void basicFloodFill(void);

#endif // FLOODFILL_H_
