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
    std::string defaultLayoutType();
    int windowBorderWidth();
    std::string headerTextFont();
    int headerTextHeight();
    int headerRowSpacing();
    int headerColumnSpacing();
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
    std::string tileTextFontImage();
    double tileTextBorderFraction();
    std::string tileTextAlignment();
    std::string tileUndeclaredWallColor();
    std::string tileUndeclaredNoWallColor();
    std::string tileIncorrectlyDeclaredWallColor();
    std::string tileIncorrectlyDeclaredNoWallColor();
    std::string mouseBodyColor();
    std::string mouseCenterOfMassColor();
    std::string mouseWheelColor();
    std::string mouseWheelSpeedIndicatorColor();
    std::string mouseSensorColor();
    std::string mouseViewColor();
    bool defaultWallTruthVisible();
    bool defaultTileColorsVisible();
    bool defaultTileFogVisible();
    bool defaultTileTextVisible();
    bool defaultTileDistanceVisible();
    bool defaultHeaderVisible();
    double tileFogAlpha();
    bool defaultWireframeMode();
    std::string distanceCorrectTileBaseColor();

    // Simulation parameters
    int randomSeed();
    double glutInitDuration();
    bool defaultPaused();
    double minSimSpeed();
    double maxSimSpeed();
    double defaultSimSpeed();
    bool collisionDetectionEnabled();
    std::string crashMessage();
    char defaultTileTextCharacter();
    double minSleepDuration();
    int mousePositionUpdateRate();
    bool printLateMousePositionUpdates();
    int collisionDetectionRate();
    bool printLateCollisionDetections();
    int numberOfCircleApproximationPoints();
    int numberOfSensorEdgePoints();
    int numberOfArchivedRuns();

    // Maze parameters
    double wallWidth();
    double wallLength();
    std::string mazeFile();
    bool useMazeFile();
    int generatedMazeWidth();
    int generatedMazeHeight();
    std::string mazeAlgorithm();
    bool saveGeneratedMaze();
    std::string generatedMazeFile();
    std::string generatedMazeType();
    bool mazeMirrored();
    int mazeRotations();

    // Mouse parameters
    std::string mouseAlgorithm();

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Graphics parameters
    int m_defaultWindowWidth;
    int m_defaultWindowHeight;
    std::string m_defaultLayoutType;
    int m_windowBorderWidth;
    std::string m_headerTextFont;
    int m_headerTextHeight;
    int m_headerRowSpacing;
    int m_headerColumnSpacing;
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
    std::string m_tileTextFontImage;
    double m_tileTextBorderFraction;
    std::string m_tileTextAlignment;
    std::string m_tileUndeclaredWallColor;
    std::string m_tileUndeclaredNoWallColor;
    std::string m_tileIncorrectlyDeclaredWallColor;
    std::string m_tileIncorrectlyDeclaredNoWallColor;
    std::string m_mouseBodyColor;
    std::string m_mouseCenterOfMassColor;
    std::string m_mouseWheelColor;
    std::string m_mouseWheelSpeedIndicatorColor;
    std::string m_mouseSensorColor;
    std::string m_mouseViewColor;
    bool m_defaultWallTruthVisible;
    bool m_defaultTileColorsVisible;
    bool m_defaultTileFogVisible;
    bool m_defaultTileTextVisible;
    bool m_defaultTileDistanceVisible;
    bool m_defaultHeaderVisible;
    double m_tileFogAlpha;
    bool m_defaultWireframeMode;
    std::string m_distanceCorrectTileBaseColor;

    // Simulation parameters
    int m_randomSeed;
    double m_glutInitDuration;
    bool m_defaultPaused;
    double m_minSimSpeed;
    double m_maxSimSpeed;
    double m_defaultSimSpeed;
    bool m_collisionDetectionEnabled;
    std::string m_crashMessage;
    char m_defaultTileTextCharacter;
    double m_minSleepDuration;
    int m_mousePositionUpdateRate;
    bool m_printLateMousePostitionUpdates;
    int m_collisionDetectionRate;
    bool m_printLateCollisionDetections;
    int m_numberOfCircleApproximationPoints;
    int m_numberOfSensorEdgePoints;
    int m_numberOfArchivedRuns;

    // Maze parameters
    double m_wallWidth;
    double m_wallLength;
    std::string m_mazeFile;
    bool m_useMazeFile;
    int m_generatedMazeWidth;
    int m_generatedMazeHeight;
    std::string m_mazeAlgorithm;
    bool m_saveGeneratedMaze;
    std::string m_generatedMazeFile;
    std::string m_generatedMazeType;
    bool m_mazeMirrored;
    int m_mazeRotations;

    // Mouse parameters
    std::string m_mouseAlgorithm;
};

} // namespace sim
