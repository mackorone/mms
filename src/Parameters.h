#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

// TODO: Figure out a nice compromise between non-const maze size and constant window size
//       Move all graphics parameters to constants, move parameters to "main.cpp", get
//       rid of this file

// Simulation parameters
static const int SLEEP_TIME = 250; // ms between simulation steps

// TODO: Doesn't work if MAZE_WIDTH and MAZE_HEIGHT are different
static const int MAZE_WIDTH = 16; // Number or tiles wide
static const int MAZE_HEIGHT = 16; // Number or tiles tall
static const std::string MAZE_FILE = "maze1.maz";

// Graphics parameters
static const int PIXELS_PER_UNIT = 4; // Choose the window size of the simulation (4 is a good size)
static const int UNITS_PER_TILE = 9; // Ensures that the ratio of wall width to length is reasonable
static const int WINDOW_WIDTH = MAZE_WIDTH * UNITS_PER_TILE * PIXELS_PER_UNIT;
static const int WINDOW_HEIGHT = MAZE_HEIGHT * UNITS_PER_TILE * PIXELS_PER_UNIT;

#endif // PARAMETERS_H_
