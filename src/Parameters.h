#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

// Simulation parameters
// TODO: Doesn't work if MAZE_WIDTH and MAZE_HEIGHT are different
static const int MAZE_WIDTH = 16; // Number or tiles wide
static const int MAZE_HEIGHT = 16; // Number or tiles tall
static const std::string MAZE_FILE = "mazeFiles/maze3.maz";

static const int UNITS_PER_TILE = 9; // Scaling factor, don't change
static const int PIXELS_PER_UNIT = 4; // NOTE: UNITS_PER_TILE * PIXELS_PER_UNIT MUST == 36
static const int WINDOW_WIDTH = MAZE_WIDTH * UNITS_PER_TILE * PIXELS_PER_UNIT;
static const int WINDOW_HEIGHT = MAZE_HEIGHT * UNITS_PER_TILE * PIXELS_PER_UNIT;
static const int SLEEP_TIME = 250; // ms between simulation steps

#endif // PARAMETERS_H_
