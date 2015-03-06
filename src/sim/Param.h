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
    std::string tileBaseColor();
    std::string tileWallColor();
    std::string tileCornerColor();
    std::string mouseBodyColor();
    std::string mouseWheelColor();
    std::string mouseSensorColor();

    // Simulation parameters
    int randomSeed();
    float glutInitTime();
    float deltaTime();
    float minSimSpeed();
    float maxSimSpeed();
    float defaultSimSpeed();
    int circleApproximationEdges();

    // Maze parameters
    std::string mazeDirectory();
    std::string mazeFile();
    bool useMazeFile();
    int randomMazeWidth();
    int randomMazeHeight();
    float wallWidth();
    float wallLength();
    float wallHeight();
    bool enforceOfficialMazeRules();
    std::string randomMazeAlgo();
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
    std::string m_tileBaseColor;
    std::string m_tileWallColor;
    std::string m_tileCornerColor;
    std::string m_mouseBodyColor;
    std::string m_mouseWheelColor;
    std::string m_mouseSensorColor;

    // Simulation parameters
    int m_randomSeed;
    float m_glutInitTime;
    float m_deltaTime;
    float m_minSimSpeed;
    float m_maxSimSpeed;
    float m_defaultSimSpeed;
    float m_circleApproximationEdges;

    // Maze parameters
    std::string m_mazeDirectory;
    std::string m_mazeFile;
    bool m_useMazeFile;
    int m_randomMazeWidth;
    int m_randomMazeHeight;
    float m_wallWidth;
    float m_wallLength;
    float m_wallHeight;
    bool m_enforceOfficialMazeRules;
    std::string m_randomMazeAlgo;
    bool m_saveRandomMaze;
};

} // namespace sim
