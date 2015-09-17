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
    int defaultWindowWidth();
    int defaultWindowHeight();
    std::string defaultLayout();
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
    std::string tileFogColor();
    std::string tileUndeclaredWallColor();
    std::string tileUndeclaredNoWallColor();
    std::string tileIncorrectlyDeclaredWallColor();
    std::string tileIncorrectlyDeclaredNoWallColor();
    std::string mouseBodyColor();
    std::string mouseWheelColor();
    std::string mouseSensorColor();
    std::string mouseViewColor();
    bool defaultWallTruthVisible();
    bool defaultTileColorsVisible();
    bool defaultTileFogVisible();
    bool defaultTileTextVisible();
    bool defaultTileDistanceVisible();
    double tileFogAlpha();
    bool defaultWireframeMode();

    // Simulation parameters
    int randomSeed();
    double glutInitDuration();
    bool defaultPaused();
    std::string crashMessage();
    double minSleepDuration();

    // TODO: MACK - Get rid of these...
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
    int numberOfArchivedRuns();

    // Maze parameters
    double wallWidth();
    double wallLength();
    double wallHeight();
    bool enforceOfficialMazeRules();
    std::string mazeFile();
    bool useMazeFile();
    int generatedMazeWidth();
    int generatedMazeHeight();
    std::string mazeAlgorithm();
    bool saveGeneratedMaze();
    bool mazeMirrored();
    int mazeRotations();

    // Mouse parameters
    std::string mouseAlgorithm();
    std::string mouseStartingDirection();

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Graphics parameters
    int m_defaultWindowWidth;
    int m_defaultWindowHeight;
    std::string m_defaultLayout;
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
    std::string m_tileFogColor;
    std::string m_tileUndeclaredWallColor;
    std::string m_tileUndeclaredNoWallColor;
    std::string m_tileIncorrectlyDeclaredWallColor;
    std::string m_tileIncorrectlyDeclaredNoWallColor;
    std::string m_mouseBodyColor;
    std::string m_mouseWheelColor;
    std::string m_mouseSensorColor;
    std::string m_mouseViewColor;
    bool m_defaultWallTruthVisible;
    bool m_defaultTileColorsVisible;
    bool m_defaultTileFogVisible;
    bool m_defaultTileTextVisible;
    bool m_defaultTileDistanceVisible;
    double m_tileFogAlpha;
    bool m_defaultWireframeMode;

    // Simulation parameters
    int m_randomSeed;
    double m_glutInitDuration;
    bool m_defaultPaused;
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
    int m_numberOfCircleApproximationPoints;
    int m_numberOfSensorEdgePoints;
    int m_numberOfArchivedRuns;

    // Maze parameters
    double m_wallWidth;
    double m_wallLength;
    double m_wallHeight;
    bool m_enforceOfficialMazeRules;
    std::string m_mazeFile;
    bool m_useMazeFile;
    int m_generatedMazeWidth;
    int m_generatedMazeHeight;
    std::string m_mazeAlgorithm;
    bool m_saveGeneratedMaze;
    bool m_mazeMirrored;
    int m_mazeRotations;

    // Mouse parameters
    std::string m_mouseAlgorithm;
    std::string m_mouseStartingDirection;
};

} // namespace sim
