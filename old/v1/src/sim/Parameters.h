#pragma once

#include <string>

namespace sim {

// Simulation Parameters
static const int SLEEP_TIME_MIN = 5; // min ms between simulation steps
static const int SLEEP_TIME_MAX = 1500; // max ms between simulation steps
static /*non-const*/ int MAZE_WIDTH = 16; // Number or tiles wide
static /*non-const*/ int MAZE_HEIGHT = 16; // Number or tiles tall
static /*non-const*/ int MAX_DISTANCE = MAZE_WIDTH*MAZE_HEIGHT; // Max distance of tile
static const std::string MAZE_FILE = ""; // Maze file to load ("maze5.maz")
static const int MIN_MAZE_STEPS = 40; // Minimum steps required to solve random maze

// Graphics parameters
static const int PIXELS_PER_UNIT = 4; // Choose the window size of the simulation (4 is a good size)
static const int UNITS_PER_TILE = 9; // Ensures that the ratio of wall width to length is reasonable
static const int WINDOW_WIDTH = MAZE_WIDTH * UNITS_PER_TILE * PIXELS_PER_UNIT;
static const int WINDOW_HEIGHT =  MAZE_HEIGHT * UNITS_PER_TILE * PIXELS_PER_UNIT;

} // namespace sim
