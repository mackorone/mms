#pragma once

#include <string>

namespace sim {

// Wrapper for the static call to Param::getInstance()
class Param;
Param* P();

class Param {

public:

    // Returns a pointer to the singleton parameter object
    static Param* getInstance();

    // Graphics parameters
    int pixelsPerMeter();
    int frameRate();
    bool printLateFrames();
    int windowWidth();
    int windowHeight();
    std::string tileBaseColor();
    std::string tileWallColor();
    std::string tileCornerColor();
    std::string tileTextColor();
    std::string tileUndeclaredWallColor();
    std::string tileUndeclaredNoWallColor();
    std::string mouseBodyColor();
    std::string mouseWheelColor();
    std::string mouseSensorColor();
    bool defaultMazeVisible();
    bool defaultMouseVisible();
    bool defaultMousePathVisible();
    bool defaultWallTruthVisible();
    bool defaultTileColorsVisible();

    // Simulation parameters
    int randomSeed();
    float glutInitDuration();
    bool defaultPaused();
    std::string crashMessage();
    float discreteInterfaceSleepDuration();
    float discreteInterfaceMinSpeed();
    float discreteInterfaceMaxSpeed();
    float discreteInterfaceDefaultSpeed();
    bool discreteInterfaceDeclareWallOnRead();
    bool declareBothWallHalves();
    int mousePositionUpdateRate();
    bool printLateMousePositionUpdates();
    int collisionDetectionRate();
    bool printLateCollisionDetections();
    bool printLateSensorReads();
    int numberOfCircleApproximationPoints();
    int numberOfSensorEdgePoints();

    // Maze parameters
    std::string mazeDirectory();
    std::string mazeFile();
    bool useMazeFile();
    float wallWidth();
    float wallLength();
    float wallHeight();
    int generatedMazeWidth();
    int generatedMazeHeight();
    bool enforceOfficialMazeRules();
    std::string mazeGenerationAlgo();
    bool saveGeneratedMaze();

    // Mouse parameters
    std::string mouseDirectory();

    // Algorithm parameters
    std::string algorithm();

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Graphics parameters
    int m_pixelsPerMeter;
    int m_frameRate;
    bool m_printLateFrames;
    std::string m_tileBaseColor;
    std::string m_tileWallColor;
    std::string m_tileCornerColor;
    std::string m_tileTextColor;
    std::string m_tileUndeclaredWallColor;
    std::string m_tileUndeclaredNoWallColor;
    std::string m_mouseBodyColor;
    std::string m_mouseWheelColor;
    std::string m_mouseSensorColor;
    bool m_defaultMazeVisible;
    bool m_defaultMouseVisible;
    bool m_defaultMousePathVisible;
    bool m_defaultWallTruthVisible;
    bool m_defaultTileColorsVisible;

    // Simulation parameters
    int m_randomSeed;
    float m_glutInitDuration;
    bool m_defaultPaused;
    std::string m_crashMessage;
    float m_discreteInterfaceSleepDuration;
    float m_discreteInterfaceMinSpeed;
    float m_discreteInterfaceMaxSpeed;
    float m_discreteInterfaceDefaultSpeed;
    bool m_discreteInterfaceDeclareWallOnRead;
    bool m_declareBothWallHalves;
    int m_mousePositionUpdateRate;
    bool m_printLateMousePostitionUpdates;
    int m_collisionDetectionRate;
    bool m_printLateCollisionDetections;
    bool m_printLateSensorReads;
    float m_numberOfCircleApproximationPoints;
    float m_numberOfSensorEdgePoints;

    // Maze parameters
    std::string m_mazeDirectory;
    std::string m_mazeFile;
    bool m_useMazeFile;
    float m_wallWidth;
    float m_wallLength;
    float m_wallHeight;
    int m_generatedMazeWidth;
    int m_generatedMazeHeight;
    bool m_enforceOfficialMazeRules;
    std::string m_mazeGenerationAlgo;
    bool m_saveGeneratedMaze;

    // Mouse parameters
    std::string m_mouseDirectory;

    // Algorithm parameters
    std::string m_algorithm;
};

} // namespace sim
