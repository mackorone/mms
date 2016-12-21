#pragma once

#include <QString>

namespace mms {

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
    QString defaultLayoutType();
    double minZoomedMapScale();
    double maxZoomedMapScale();
    double defaultZoomedMapScale();
    bool defaultRotateZoomedMap();
    QString tileBaseColor();
    QString tileWallColor();
    QString tileCornerColor();
    QString tileFogColor();
    QString tileTextFontImage();
    double tileTextBorderFraction();
    QString tileTextAlignment();
    QString tileUndeclaredWallColor();
    QString tileUndeclaredNoWallColor();
    QString tileIncorrectlyDeclaredWallColor();
    QString tileIncorrectlyDeclaredNoWallColor();
    QString mouseBodyColor();
    QString mouseCenterOfMassColor();
    QString mouseWheelColor();
    QString mouseWheelSpeedIndicatorColor();
    QString mouseSensorColor();
    QString mouseViewColor();
    bool defaultWallTruthVisible();
    bool defaultTileColorsVisible();
    bool defaultTileFogVisible();
    bool defaultTileTextVisible();
    bool defaultTileDistanceVisible();
    double tileFogAlpha();
    QString distanceCorrectTileBaseColor();

    // Simulation parameters
    int randomSeed();
    bool defaultPaused();
    double minSimSpeed();
    double maxSimSpeed();
    double defaultSimSpeed();
    bool collisionDetectionEnabled();
    QString crashMessage();
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
    QString mazeFile();
    bool useMazeFile();
    int generatedMazeWidth();
    int generatedMazeHeight();
    QString mazeAlgorithm();
    bool saveGeneratedMaze();
    QString generatedMazeFile();
    QString generatedMazeType();
    bool mazeMirrored();
    int mazeRotations();

    // Mouse parameters
    QString mouseAlgorithm();

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Graphics parameters
    int m_defaultWindowWidth;
    int m_defaultWindowHeight;
    QString m_defaultLayoutType;
    double m_minZoomedMapScale;
    double m_maxZoomedMapScale;
    double m_defaultZoomedMapScale;
    bool m_defaultRotateZoomedMap;
    QString m_tileBaseColor;
    QString m_tileWallColor;
    QString m_tileCornerColor;
    QString m_tileFogColor;
    QString m_tileTextFontImage;
    double m_tileTextBorderFraction;
    QString m_tileTextAlignment;
    QString m_tileUndeclaredWallColor;
    QString m_tileUndeclaredNoWallColor;
    QString m_tileIncorrectlyDeclaredWallColor;
    QString m_tileIncorrectlyDeclaredNoWallColor;
    QString m_mouseBodyColor;
    QString m_mouseCenterOfMassColor;
    QString m_mouseWheelColor;
    QString m_mouseWheelSpeedIndicatorColor;
    QString m_mouseSensorColor;
    QString m_mouseViewColor;
    bool m_defaultWallTruthVisible;
    bool m_defaultTileColorsVisible;
    bool m_defaultTileFogVisible;
    bool m_defaultTileTextVisible;
    bool m_defaultTileDistanceVisible;
    double m_tileFogAlpha;
    QString m_distanceCorrectTileBaseColor;

    // Simulation parameters
    int m_randomSeed;
    bool m_defaultPaused;
    double m_minSimSpeed;
    double m_maxSimSpeed;
    double m_defaultSimSpeed;
    bool m_collisionDetectionEnabled;
    QString m_crashMessage;
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
    QString m_mazeFile;
    bool m_useMazeFile;
    int m_generatedMazeWidth;
    int m_generatedMazeHeight;
    QString m_mazeAlgorithm;
    bool m_saveGeneratedMaze;
    QString m_generatedMazeFile;
    QString m_generatedMazeType;
    bool m_mazeMirrored;
    int m_mazeRotations;

    // Mouse parameters
    QString m_mouseAlgorithm;
};

} // namespace mms
