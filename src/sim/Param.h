#pragma once

#include <string>

namespace sim {

// Wrapper for the static call to Param::getInstance()
class Param;
Param* P();

class Param {

public:

    // Returns a pointer to the singeton parameter object
    static Param* getInstance();

    // Graphics parameters
    int pixelsPerMeter();
    int frameRate();      // Frames per second
    int windowWidth();    // Number of pixels
    int windowHeight();   // Number of pixels

    // Simulation parameters
    int minSimSpeed(); // min ms between simulation steps (TODO: Steps isn't really going to be a thing....)
    int maxSimSpeed(); // max ms between simulation steps (TODO: Change these descriptions ... )
    // TODO: max sim speed = min sleep... we need to fix this bug

    // Maze parameters
    std::string mazeDirectory(); // Maze file to load ("maze5.maz") // TODO Description
    std::string mazeFile();      // Maze file to load ("maze5.maz")
    int mazeWidth();             // Number of tiles
    int mazeHeight();            // Number of tiles
    float wallWidth();
    float wallLength();
    float wallHeight();
    float wallTolerance();
    int minSolutionLength();     // Minimum steps required to solve random maze // TODO: This should be in the randomize code.... right?
    bool saveRandomMaze();       // Whether or not we should same the random maze

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Graphics parameters
    int m_pixelsPerMeter;
    int m_frameRate;
    int m_windowWidth;
    int m_windowHeight;

    // Simulation parameters
    float m_minSimSpeed;
    float m_maxSimSpeed;

    // Maze parameters
    std::string m_mazeDirectory;
    std::string m_mazeFile;
    int m_mazeWidth;
    int m_mazeHeight;
    float m_wallWidth;
    float m_wallLength;
    float m_wallHeight;
    float m_wallTolerance;
    int m_minSolutionLength;
    bool m_saveRandomMaze;
};

} // namespace sim
