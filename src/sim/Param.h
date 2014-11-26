#pragma once

#include <string>

namespace sim {

// TODO: This uses the singleton pattern

// Wrapper for the static call to Param::getInstance()
class Param;
Param* P();

// A parameters class
class Param {

public:

    // Returns a pointer to the singeton parameter object
    static Param* getInstance();

    int SLEEP_TIME_MIN();   // min ms between simulation steps (TODO: Steps isn't really going to be a thing....)
    int SLEEP_TIME_MAX();   // max ms between simulation steps
    int MAZE_WIDTH();       // Number or tiles wide
    int MAZE_HEIGHT();      // Number or tiles tall
    int MAX_DISTANCE();     // Max distance of tile
    int MIN_MAZE_STEPS();   // Minimum steps required to solve random maze
    std::string MAZE_FILE();// Maze file to load ("maze5.maz")

    // Graphics parameters
    int PIXELS_PER_UNIT();  // Choose the window size of the simulation (4 is a good size)
    int UNITS_PER_TILE();   // Ensures that the ratio of wall width to length is reasonable
    int WINDOW_WIDTH();
    int WINDOW_HEIGHT();

private:

    // TODO: Document the singleton pattern
    Param();
    // A pointer to the actual instance of the parameter object
    static Param* INSTANCE;

    // Constants
    int m_SLEEP_TIME_MIN; // min ms between simulation steps (TODO: Steps isn't really going to be a thing....)
    int m_SLEEP_TIME_MAX; // max ms between simulation steps
    int m_MAZE_WIDTH; // Number or tiles wide
    int m_MAZE_HEIGHT; // Number or tiles tall
    int m_MAX_DISTANCE; // Max distance of tile
    std::string m_MAZE_FILE; // Maze file to load ("maze5.maz")
    int m_MIN_MAZE_STEPS; // Minimum steps required to solve random maze

    // Graphics parameters
    int m_PIXELS_PER_UNIT; // Choose the window size of the simulation (4 is a good size)
    int m_UNITS_PER_TILE; // Ensures that the ratio of wall width to length is reasonable
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
};

} // namespace sim
