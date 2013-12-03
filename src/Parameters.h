#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

// TODO: Figure out how to get rid of this file

// Simulation Parameters
static /*non-const*/ bool PAUSED = false; // ms between simulation steps
static /*non-const*/ int SLEEP_TIME = 150; // ms between simulation steps
static const int SLEEP_TIME_MIN = 5; // min ms between simulation steps
static const int SLEEP_TIME_MAX = 1500; // max ms between simulation steps
static const int MAZE_WIDTH = 16; // Number or tiles wide
static const int MAZE_HEIGHT = 16; // Number or tiles tall
static const std::string MAZE_FILE = "maze2.maz";

// Graphics parameters
static const int PIXELS_PER_UNIT = 4; // Choose the window size of the simulation (4 is a good size)
static const int UNITS_PER_TILE = 9; // Ensures that the ratio of wall width to length is reasonable
static const int WINDOW_WIDTH = MAZE_WIDTH * UNITS_PER_TILE * PIXELS_PER_UNIT;
static const int WINDOW_HEIGHT =  MAZE_HEIGHT * UNITS_PER_TILE * PIXELS_PER_UNIT;

#endif // PARAMETERS_H_
