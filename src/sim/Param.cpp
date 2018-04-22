#include "Param.h"

#include "Direction.h"
#include "LayoutType.h"
#include "Logging.h"
#include "MazeFileType.h"
#include "ParamParser.h"
#include "Resources.h"
#include "TileTextAlignment.h"

namespace mms {

Param* P() {
    return Param::getInstance();
}

// Definition of the variable for linking
Param* Param::INSTANCE = nullptr;
Param* Param::getInstance() {
    if (nullptr == INSTANCE) {
        INSTANCE = new Param();
    }
    return INSTANCE;
}

Param::Param() {

    // Graphical Parameters
    m_defaultWindowWidth = ParamParser::getIntIfHasIntAndNotLessThan(
        "default-window-width", 1200, 100);
    m_defaultWindowHeight = ParamParser::getIntIfHasIntAndNotLessThan(
        "default-window-height", 633, 100);
    m_defaultLayoutType = ParamParser::getStringIfHasStringAndIsLayoutType(
        "default-layout-type", LAYOUT_TYPE_TO_STRING().value(LayoutType::FULL));
    m_minZoomedMapScale = ParamParser::getDoubleIfHasDoubleAndInRange(
        "min-zoomed-map-scale", 0.02, 0.01, 0.04);
    m_maxZoomedMapScale = ParamParser::getDoubleIfHasDoubleAndInRange(
        "max-zoomed-map-scale", 1.0, 0.5, 2.0);
    m_defaultZoomedMapScale = ParamParser::getDoubleIfHasDoubleAndInRange(
        "default-zoomed-map-scale", 0.1, m_minZoomedMapScale, m_maxZoomedMapScale);
    m_defaultRotateZoomedMap = ParamParser::getBoolIfHasBool(
        "default-rotate-zoomed-map", false);
    m_tileTextFontImage = ParamParser::getStringIfHasString(
        "tile-text-font-image", Resources::getFonts().last());
    m_tileTextBorderFraction = ParamParser::getDoubleIfHasDoubleAndInRange(
        "tile-text-border-fraction", .05, .00, .50);
    m_tileTextAlignment = ParamParser::getStringIfHasStringAndIsTileTextAlignment(
        "tile-text-alignment", TILE_TEXT_ALIGNMENT_TO_STRING().value(TileTextAlignment::CENTER_CENTER));
    m_defaultWallTruthVisible = ParamParser::getBoolIfHasBool(
        "default-wall-truth-visible", true);
    m_defaultTileColorsVisible = ParamParser::getBoolIfHasBool(
        "default-tile-colors-visible", true);
    m_defaultTileFogVisible = ParamParser::getBoolIfHasBool(
        "default-tile-fog-visible", false);
    m_defaultTileTextVisible = ParamParser::getBoolIfHasBool(
        "default-tile-text-visible", true);
    m_defaultTileDistanceVisible = ParamParser::getBoolIfHasBool(
        "default-tile-distance-visible", false);

    // Simulation Parameters
    bool useRandomSeed = ParamParser::getBoolIfHasBool(
        "use-random-seed", false);
    if (useRandomSeed && !ParamParser::hasIntValue("random-seed")) {
        qWarning().noquote().nospace()
            << "The value of use-random-seed is true but no valid random-seed"
            << " value was provided. Setting \"use-random-seed\" to false.";
        useRandomSeed = false;
    }
    m_randomSeed = (useRandomSeed ? ParamParser::getIntValue("random-seed") : std::random_device()());
    m_defaultPaused = ParamParser::getBoolIfHasBool(
        "default-paused", false);
    m_maxSimSpeed = ParamParser::getDoubleIfHasDoubleAndInRange(
        "max-sim-speed", 10.0, 2.0, 100.0);
    m_defaultSimSpeed = ParamParser::getDoubleIfHasDoubleAndInRange(
        "default-sim-speed", 1.0, 0.0, m_maxSimSpeed);
    m_collisionDetectionEnabled = ParamParser::getBoolIfHasBool(
        "collision-detection-enabled", true);
    m_crashMessage = ParamParser::getStringIfHasString(
        "crash-message", "CRASH");
    m_defaultTileTextCharacter = ParamParser::getCharIfHasChar(
        "default-tile-text-character", '?');
    m_minSleepDuration = ParamParser::getDoubleIfHasDoubleAndInRange(
        "min-sleep-duration", 5, 1, 25);
    m_mousePositionUpdateRate = ParamParser::getIntIfHasIntAndInRange(
        "mouse-position-update-rate", 100, 1, 2000);
    m_printLateMousePostitionUpdates = ParamParser::getBoolIfHasBool(
        "print-late-mouse-position-updates", true);
    m_collisionDetectionRate = ParamParser::getIntIfHasIntAndInRange(
        "collision-detection-rate", 40, 1, 100);
    m_printLateCollisionDetections = ParamParser::getBoolIfHasBool(
        "print-late-collision-detections", true);
    m_numberOfCircleApproximationPoints = ParamParser::getIntIfHasIntAndInRange(
        "number-of-circle-approximation-points", 8, 3, 30);
    m_numberOfSensorEdgePoints = ParamParser::getIntIfHasIntAndInRange(
        "number-of-sensor-edge-points", 3, 2, 10);

    // Maze Parameters
    m_wallWidth = ParamParser::getDoubleIfHasDoubleAndInRange(
        "wall-width", 0.012, 0.006, 0.024);
    m_wallLength = ParamParser::getDoubleIfHasDoubleAndInRange(
        "wall-length", 0.168, 0.084, 0.336);
    m_mazeMirrored = ParamParser::getBoolIfHasBool(
        "maze-mirrored", false);
    m_mazeRotations = ParamParser::getIntIfHasIntAndInRange(
        "maze-rotations", 0, 0, 3);
}

int Param::defaultWindowWidth() {
    return m_defaultWindowWidth;
}

int Param::defaultWindowHeight() {
    return m_defaultWindowHeight;
}

QString Param::tileTextFontImage() {
    return m_tileTextFontImage;
}

double Param::tileTextBorderFraction() {
    return m_tileTextBorderFraction;
}

QString Param::tileTextAlignment() {
    return m_tileTextAlignment;
}

int Param::randomSeed() {
    return m_randomSeed;
}

double Param::maxSimSpeed() {
    return m_maxSimSpeed;
}

double Param::defaultSimSpeed() {
    return m_defaultSimSpeed;
}

char Param::defaultTileTextCharacter() {
    return m_defaultTileTextCharacter;
}

double Param::minSleepDuration() {
    return m_minSleepDuration;
}

int Param::numberOfCircleApproximationPoints() {
    return m_numberOfCircleApproximationPoints;
}

int Param::numberOfSensorEdgePoints() {
    return m_numberOfSensorEdgePoints;
}

double Param::wallWidth() {
    return m_wallWidth;
}

double Param::wallLength() {
    return m_wallLength;
}

bool Param::mazeMirrored() {
    return m_mazeMirrored;
}

int Param::mazeRotations() {
    return m_mazeRotations;
}

} // namespace mms
