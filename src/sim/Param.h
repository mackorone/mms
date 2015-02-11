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
    int frameRate();
    int windowWidth();
    int windowHeight();

    // Simulation parameters
    int glutInitTime();
    int minSimSpeed();
    int maxSimSpeed();

    // Maze parameters
    std::string mazeDirectory();
    std::string mazeFile();
    int mazeWidth();
    int mazeHeight();
    float wallWidth();
    float wallLength();
    float wallHeight();
    float wallTolerance();
    int minSolutionLength();
    bool saveRandomMaze();

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
    float m_glutInitTime;
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
