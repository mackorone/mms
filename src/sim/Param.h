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
    // QString defaultLayoutType();
    // double minZoomedMapScale();
    // double maxZoomedMapScale();
    // double defaultZoomedMapScale();
    // bool defaultRotateZoomedMap(); 
    QString tileTextFontImage(); // TODO: MACK - delete this
    double tileTextBorderFraction(); // TODO: MACK - delete this
    QString tileTextAlignment();
    // bool defaultWallTruthVisible();
    // bool defaultTileColorsVisible();
    // bool defaultTileFogVisible();
    // bool defaultTileTextVisible();
    // bool defaultTileDistanceVisible();

    // Simulation parameters
    int randomSeed(); // TODO: MACK - delete this - no need for random nums
    // bool defaultPaused();
    double maxSimSpeed();
    double defaultSimSpeed();
    // bool collisionDetectionEnabled();
    // QString crashMessage();
    double minSleepDuration();
    // int mousePositionUpdateRate();
    // bool printLateMousePositionUpdates();
    // int collisionDetectionRate();
    // bool printLateCollisionDetections();
    int numberOfCircleApproximationPoints();
    int numberOfSensorEdgePoints();

    // Maze parameters
    double wallWidth();
    double wallLength();
    bool mazeMirrored(); // TODO: MACK
    int mazeRotations(); // TODO: MACK

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Graphics parameters
    QString m_defaultLayoutType;
    double m_minZoomedMapScale;
    double m_maxZoomedMapScale;
    double m_defaultZoomedMapScale;
    bool m_defaultRotateZoomedMap;
    QString m_tileTextFontImage;
    double m_tileTextBorderFraction;
    QString m_tileTextAlignment;
    bool m_defaultWallTruthVisible;
    bool m_defaultTileColorsVisible;
    bool m_defaultTileFogVisible;
    bool m_defaultTileTextVisible;
    bool m_defaultTileDistanceVisible;

    // Simulation parameters
    int m_randomSeed;
    bool m_defaultPaused;
    double m_maxSimSpeed;
    double m_defaultSimSpeed;
    bool m_collisionDetectionEnabled;
    QString m_crashMessage;
    double m_minSleepDuration;
    int m_mousePositionUpdateRate;
    bool m_printLateMousePostitionUpdates;
    int m_collisionDetectionRate;
    bool m_printLateCollisionDetections;
    int m_numberOfCircleApproximationPoints;
    int m_numberOfSensorEdgePoints;

    // Maze parameters
    double m_wallWidth;
    double m_wallLength;
    bool m_mazeMirrored;
    int m_mazeRotations;
};

} // namespace mms
