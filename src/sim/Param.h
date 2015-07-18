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
    int initialWindowWidth();
    int initialWindowHeight();
    int windowBorderWidth();
    double minZoomedMapScale();
    double maxZoomedMapScale();
    double defaultZoomedMapScale();
    bool defaultRotateZoomedMap();
    int frameRate();
    bool printLateFrames();
    std::string tileBaseColor();
    std::string tileWallColor();
    std::string tileCornerColor();
    std::string tileTextColor();
    std::string tileFogColor();
    std::string tileUndeclaredWallColor();
    std::string tileUndeclaredNoWallColor();
    std::string tileIncorrectlyDeclaredWallColor();
    std::string tileIncorrectlyDeclaredNoWallColor();
    std::string mouseBodyColor();
    std::string mouseWheelColor();
    std::string mouseSensorColor();
    std::string mouseViewColor();
    bool defaultMousePathVisible();
    bool defaultWallTruthVisible();
    bool defaultTileColorsVisible();
    bool defaultTileTextVisible();
    bool defaultTileFogVisible();
    double tileFogAlpha();
    bool defaultWireframeMode();

    // Simulation parameters
    int randomSeed();
    double glutInitDuration();
    bool defaultPaused();
    int printWidth();
    std::string printIndentString();
    std::string crashMessage();
    double minSleepDuration();
    double discreteInterfaceMinSpeed();
    double discreteInterfaceMaxSpeed();
    double discreteInterfaceDefaultSpeed();
    bool discreteInterfaceDeclareWallOnRead();
    bool discreteInterfaceUnfogTileOnEntry();
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
    double wallWidth();
    double wallLength();
    double wallHeight();
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
    int m_initialWindowWidth;
    int m_initialWindowHeight;
    int m_windowBorderWidth;
    double m_minZoomedMapScale;
    double m_maxZoomedMapScale;
    double m_defaultZoomedMapScale;
    bool m_defaultRotateZoomedMap;
    int m_frameRate;
    bool m_printLateFrames;
    std::string m_tileBaseColor;
    std::string m_tileWallColor;
    std::string m_tileCornerColor;
    std::string m_tileTextColor;
    std::string m_tileFogColor;
    std::string m_tileUndeclaredWallColor;
    std::string m_tileUndeclaredNoWallColor;
    std::string m_tileIncorrectlyDeclaredWallColor;
    std::string m_tileIncorrectlyDeclaredNoWallColor;
    std::string m_mouseBodyColor;
    std::string m_mouseWheelColor;
    std::string m_mouseSensorColor;
    std::string m_mouseViewColor;
    bool m_defaultMousePathVisible;
    bool m_defaultWallTruthVisible;
    bool m_defaultTileColorsVisible;
    bool m_defaultTileTextVisible;
    bool m_defaultTileFogVisible;
    double m_tileFogAlpha;
    bool m_defaultWireframeMode;

    // Simulation parameters
    int m_randomSeed;
    double m_glutInitDuration;
    bool m_defaultPaused;
    int m_printWidth;
    std::string m_printIdentString;
    std::string m_crashMessage;
    double m_minSleepDuration;
    double m_discreteInterfaceMinSpeed;
    double m_discreteInterfaceMaxSpeed;
    double m_discreteInterfaceDefaultSpeed;
    bool m_discreteInterfaceDeclareWallOnRead;
    bool m_discreteInterfaceUnfogTileOnEntry;
    bool m_declareBothWallHalves;
    int m_mousePositionUpdateRate;
    bool m_printLateMousePostitionUpdates;
    int m_collisionDetectionRate;
    bool m_printLateCollisionDetections;
    bool m_printLateSensorReads;
    double m_numberOfCircleApproximationPoints;
    double m_numberOfSensorEdgePoints;

    // Maze parameters
    std::string m_mazeDirectory;
    std::string m_mazeFile;
    bool m_useMazeFile;
    double m_wallWidth;
    double m_wallLength;
    double m_wallHeight;
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
