#include "Param.h"

#include <limits>
#include <random>

#include "Color.h"
#include "Direction.h"
#include "Directory.h"
#include "LayoutType.h"
#include "Logging.h"
#include "MazeFileType.h"
#include "ParamParser.h"
#include "TileTextAlignment.h"

namespace sim {

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

    // Create the parameter parser object
    ParamParser parser(Directory::getResDirectory() + "parameters.xml");

    // Graphical Parameters
    m_defaultWindowWidth = parser.getIntIfHasIntAndNotLessThan(
        "default-window-width", 990, 100);
    m_defaultWindowHeight = parser.getIntIfHasIntAndNotLessThan(
        "default-window-height", 700, 100);
    m_defaultLayoutType = parser.getStringIfHasStringAndIsLayoutType(
        "default-layout-type", LAYOUT_TYPE_TO_STRING.at(LayoutType::BOTH));
    m_windowBorderWidth = parser.getIntIfHasIntAndInRange(
        "window-border-width", 10, 0, 30);
    m_headerTextFont = parser.getStringIfHasString(
        "header-text-font", "Unispace-Bold.ttf");
    m_headerTextHeight = parser.getIntIfHasIntAndInRange(
        "header-text-height", 9, 0, 30);
    m_headerRowSpacing = parser.getIntIfHasIntAndInRange(
        "header-row-spacing", 6, 0, 30);
    m_headerColumnSpacing = parser.getIntIfHasIntAndInRange(
        "header-column-spacing", 20, 0, 60);
    m_minZoomedMapScale = parser.getDoubleIfHasDoubleAndInRange(
        "min-zoomed-map-scale", 0.02, 0.01, 0.04);
    m_maxZoomedMapScale = parser.getDoubleIfHasDoubleAndInRange(
        "max-zoomed-map-scale", 1.0, 0.5, 2.0);
    m_defaultZoomedMapScale = parser.getDoubleIfHasDoubleAndInRange(
        "default-zoomed-map-scale", 0.1, m_minZoomedMapScale, m_maxZoomedMapScale);
    m_defaultRotateZoomedMap = parser.getBoolIfHasBool(
        "default-rotate-zoomed-map", false);
    m_frameRate = parser.getIntIfHasIntAndInRange(
        "frame-rate", 60, 1, 120);
    m_printLateFrames = parser.getBoolIfHasBool(
        "print-late-frames", false);
    m_tileBaseColor = parser.getStringIfHasStringAndIsColor(
        "tile-base-color", COLOR_TO_STRING.at(Color::BLACK));
    m_tileWallColor = parser.getStringIfHasStringAndIsColor(
        "tile-wall-color", COLOR_TO_STRING.at(Color::RED));
    m_tileCornerColor = parser.getStringIfHasStringAndIsColor(
        "tile-corner-color", COLOR_TO_STRING.at(Color::GRAY));
    m_tileFogColor = parser.getStringIfHasStringAndIsColor(
        "tile-fog-color", COLOR_TO_STRING.at(Color::GRAY));
    m_tileTextFontImage = parser.getStringIfHasString(
        "tile-text-font-image", "Unispace-Bold.png");
    m_tileTextBorderFraction = parser.getDoubleIfHasDoubleAndInRange(
        "tile-text-border-fraction", .05, .00, .50);
    m_tileTextAlignment = parser.getStringIfHasStringAndIsTileTextAlignment(
        "tile-text-alignment", TILE_TEXT_ALIGNMENT_TO_STRING.at(TileTextAlignment::CENTER_CENTER));
    m_tileUndeclaredWallColor = parser.getStringIfHasStringAndIsColor(
        "tile-undeclared-wall-color", COLOR_TO_STRING.at(Color::DARK_RED));
    m_tileUndeclaredNoWallColor = parser.getStringIfHasStringAndIsColor(
        "tile-undeclared-no-wall-color", COLOR_TO_STRING.at(Color::DARK_GRAY));
    m_tileIncorrectlyDeclaredWallColor = parser.getStringIfHasStringAndIsColor(
        "tile-incorrectly-declared-wall-color", COLOR_TO_STRING.at(Color::ORANGE));
    m_tileIncorrectlyDeclaredNoWallColor = parser.getStringIfHasStringAndIsColor(
        "tile-incorrectly-declared-no-wall-color", COLOR_TO_STRING.at(Color::DARK_CYAN));
    m_mouseBodyColor = parser.getStringIfHasStringAndIsColor(
        "mouse-body-color", COLOR_TO_STRING.at(Color::BLUE));
    m_mouseCenterOfMassColor = parser.getStringIfHasStringAndIsColor(
        "mouse-center-of-mass-color", COLOR_TO_STRING.at(Color::ORANGE));
    m_mouseWheelColor = parser.getStringIfHasStringAndIsColor(
        "mouse-wheel-color", COLOR_TO_STRING.at(Color::GREEN));
    m_mouseWheelSpeedIndicatorColor = parser.getStringIfHasStringAndIsColor(
        "mouse-wheel-speed-indicator-color", COLOR_TO_STRING.at(Color::DARK_GREEN));
    m_mouseSensorColor = parser.getStringIfHasStringAndIsColor(
        "mouse-sensor-color", COLOR_TO_STRING.at(Color::GREEN));
    m_mouseViewColor = parser.getStringIfHasStringAndIsColor(
        "mouse-view-color", COLOR_TO_STRING.at(Color::WHITE));
    m_defaultWallTruthVisible = parser.getBoolIfHasBool(
        "default-wall-truth-visible", false);
    m_defaultTileColorsVisible = parser.getBoolIfHasBool(
        "default-tile-colors-visible", true);
    m_defaultTileFogVisible = parser.getBoolIfHasBool(
        "default-tile-fog-visible", true);
    m_defaultTileTextVisible = parser.getBoolIfHasBool(
        "default-tile-text-visible", true);
    m_defaultTileDistanceVisible = parser.getBoolIfHasBool(
        "default-tile-distance-visible", true);
    m_defaultHeaderVisible = parser.getBoolIfHasBool(
        "default-header-visible", true);
    m_tileFogAlpha = parser.getDoubleIfHasDoubleAndInRange(
        "tile-fog-alpha", 0.15, 0.0, 1.0);
    m_defaultWireframeMode = parser.getBoolIfHasBool(
        "default-wireframe-mode", false);
    m_distanceCorrectTileBaseColor = parser.getStringIfHasStringAndIsColor(
        "distance-correct-tile-base-color", COLOR_TO_STRING.at(Color::DARK_YELLOW));

    // Simulation Parameters
    bool useRandomSeed = parser.getBoolIfHasBool(
        "use-random-seed", false);
    if (useRandomSeed && !parser.hasIntValue("random-seed")) {
        L()->warn(
            "The value of use-random-seed is true but no valid random-seed "
            "value was provided. Setting \"use-random-seed\" to false.");
        useRandomSeed = false;
    }
    m_randomSeed = (useRandomSeed ? parser.getIntValue("random-seed") : std::random_device()());
    m_glutInitDuration = parser.getDoubleIfHasDoubleAndInRange(
        "glut-init-duration", 0.1, 0.0, 5.0);
    m_defaultPaused = parser.getBoolIfHasBool(
        "default-paused", false);
    m_minSimSpeed = parser.getDoubleIfHasDoubleAndInRange(
        "min-sim-speed", 0.1, 0.01, 0.5);
    m_maxSimSpeed = parser.getDoubleIfHasDoubleAndInRange(
        "max-sim-speed", 10.0, 2.0, 100.0);
    m_defaultSimSpeed = parser.getDoubleIfHasDoubleAndInRange(
        "default-sim-speed", 1.0, m_minSimSpeed, m_maxSimSpeed);
    m_collisionDetectionEnabled = parser.getBoolIfHasBool(
        "collision-detection-enabled", true);
    m_crashMessage = parser.getStringIfHasString(
        "crash-message", "CRASH");
    m_defaultTileTextCharacter = parser.getCharIfHasChar(
        "default-tile-text-character", '?');
    m_minSleepDuration = parser.getDoubleIfHasDoubleAndInRange(
        "min-sleep-duration", 5, 1, 25);
    m_mousePositionUpdateRate = parser.getIntIfHasIntAndInRange(
        "mouse-position-update-rate", 500, 1, 2000);
    m_printLateMousePostitionUpdates = parser.getBoolIfHasBool(
        "print-late-mouse-position-updates", false);
    m_collisionDetectionRate = parser.getIntIfHasIntAndInRange(
        "collision-detection-rate", 40, 1, 100);
    m_printLateCollisionDetections = parser.getBoolIfHasBool(
        "print-late-collision-detections", false);
    m_numberOfCircleApproximationPoints = parser.getIntIfHasIntAndInRange(
        "number-of-circle-approximation-points", 8, 3, 30);
    m_numberOfSensorEdgePoints = parser.getIntIfHasIntAndInRange(
        "number-of-sensor-edge-points", 3, 2, 10);
    m_numberOfArchivedRuns = parser.getIntIfHasIntAndInRange(
        "number-of-archived-runs", 20, 1, 1000);

    // Maze Parameters
    m_wallWidth = parser.getDoubleIfHasDoubleAndInRange(
        "wall-width", 0.012, 0.006, 0.024);
    m_wallLength = parser.getDoubleIfHasDoubleAndInRange(
        "wall-length", 0.168, 0.084, 0.336);
    m_mazeFile = parser.getStringIfHasString(
        "maze-file", "");
    m_useMazeFile = parser.getBoolIfHasBool(
        "use-maze-file", false);
    m_generatedMazeWidth = parser.getIntIfHasIntAndInRange(
        "generated-maze-width", 16, 1, 256);
    m_generatedMazeHeight = parser.getIntIfHasIntAndInRange(
        "generated-maze-height", 16, 1, 256);
    m_mazeAlgorithm = parser.getStringIfHasString(
        "maze-algorithm", "Tomasz");
    m_saveGeneratedMaze = parser.getBoolIfHasBool(
        "save-generated-maze", true);
    m_generatedMazeFile = parser.getStringIfHasString(
        "generated-maze-file", "auto_generated_maze");
    m_generatedMazeType = parser.getStringIfHasStringAndIsMazeFileType(
        "generated-maze-type", MAZE_FILE_TYPE_TO_STRING.at(MazeFileType::MAP));
    m_mazeMirrored = parser.getBoolIfHasBool(
        "maze-mirrored", false);
    m_mazeRotations = parser.getIntIfHasIntAndInRange(
        "maze-rotations", 0, 0, 3);

    // Mouse parameters
    m_mouseAlgorithm = parser.getStringIfHasString(
        "mouse-algorithm", "RightWallFollow");
}

int Param::defaultWindowWidth() {
    return m_defaultWindowWidth;
}

int Param::defaultWindowHeight() {
    return m_defaultWindowHeight;
}

std::string Param::defaultLayoutType() {
    return m_defaultLayoutType;
}

int Param::windowBorderWidth() {
    return m_windowBorderWidth;
}

std::string Param::headerTextFont() {
    return m_headerTextFont;
}

int Param::headerTextHeight() {
    return m_headerTextHeight;
}

int Param::headerRowSpacing() {
    return m_headerRowSpacing;
}

int Param::headerColumnSpacing() {
    return m_headerColumnSpacing;
}

double Param::minZoomedMapScale() {
    return m_minZoomedMapScale;
}

double Param::maxZoomedMapScale() {
    return m_maxZoomedMapScale;
}

double Param::defaultZoomedMapScale() {
    return m_defaultZoomedMapScale;
}

bool Param::defaultRotateZoomedMap() {
    return m_defaultRotateZoomedMap;
}

int Param::frameRate() {
    return m_frameRate;
}

bool Param::printLateFrames() {
    return m_printLateFrames;
}

std::string Param::tileBaseColor() {
    return m_tileBaseColor;
}

std::string Param::tileWallColor() {
    return m_tileWallColor;
}

std::string Param::tileCornerColor() {
    return m_tileCornerColor;
}

std::string Param::tileFogColor() {
    return m_tileFogColor;
}

std::string Param::tileTextFontImage() {
    return m_tileTextFontImage;
}

double Param::tileTextBorderFraction() {
    return m_tileTextBorderFraction;
}

std::string Param::tileTextAlignment() {
    return m_tileTextAlignment;
}

std::string Param::tileUndeclaredWallColor() {
    return m_tileUndeclaredWallColor;
}

std::string Param::tileUndeclaredNoWallColor() {
    return m_tileUndeclaredNoWallColor;
}

std::string Param::tileIncorrectlyDeclaredWallColor() {
    return m_tileIncorrectlyDeclaredWallColor;
}

std::string Param::tileIncorrectlyDeclaredNoWallColor() {
    return m_tileIncorrectlyDeclaredNoWallColor;
}

std::string Param::mouseBodyColor() {
    return m_mouseBodyColor;
}

std::string Param::mouseCenterOfMassColor() {
    return m_mouseCenterOfMassColor;
}

std::string Param::mouseWheelColor() {
    return m_mouseWheelColor;
}

std::string Param::mouseWheelSpeedIndicatorColor() {
    return m_mouseWheelSpeedIndicatorColor;
}

std::string Param::mouseSensorColor() {
    return m_mouseSensorColor;
}

std::string Param::mouseViewColor() {
    return m_mouseViewColor;
}

bool Param::defaultWallTruthVisible() {
    return m_defaultWallTruthVisible;
}

bool Param::defaultTileColorsVisible() {
    return m_defaultTileColorsVisible;
}

bool Param::defaultTileFogVisible() {
    return m_defaultTileFogVisible;
}

bool Param::defaultTileTextVisible() {
    return m_defaultTileTextVisible;
}

bool Param::defaultTileDistanceVisible() {
    return m_defaultTileDistanceVisible;
}

bool Param::defaultHeaderVisible() {
    return m_defaultHeaderVisible;
}

double Param::tileFogAlpha() {
    return m_tileFogAlpha;
}

bool Param::defaultWireframeMode() {
    return m_defaultWireframeMode;
}

std::string Param::distanceCorrectTileBaseColor() {
    return m_distanceCorrectTileBaseColor;
}

int Param::randomSeed() {
    return m_randomSeed;
}

double Param::glutInitDuration() {
    return m_glutInitDuration;
}

bool Param::defaultPaused() {
    return m_defaultPaused;
}

double Param::minSimSpeed() {
    return m_minSimSpeed;
}

double Param::maxSimSpeed() {
    return m_maxSimSpeed;
}

double Param::defaultSimSpeed() {
    return m_defaultSimSpeed;
}

bool Param::collisionDetectionEnabled() {
    return m_collisionDetectionEnabled;
}

std::string Param::crashMessage() {
    return m_crashMessage;
}

char Param::defaultTileTextCharacter() {
    return m_defaultTileTextCharacter;
}

double Param::minSleepDuration() {
    return m_minSleepDuration;
}

int Param::mousePositionUpdateRate() {
    return m_mousePositionUpdateRate;
}

bool Param::printLateMousePositionUpdates() {
    return m_printLateMousePostitionUpdates;
}

int Param::collisionDetectionRate() {
    return m_collisionDetectionRate;
}

bool Param::printLateCollisionDetections() {
    return m_printLateCollisionDetections;
}

int Param::numberOfCircleApproximationPoints() {
    return m_numberOfCircleApproximationPoints;
}

int Param::numberOfSensorEdgePoints() {
    return m_numberOfSensorEdgePoints;
}

int Param::numberOfArchivedRuns() {
    return m_numberOfArchivedRuns;
}

std::string Param::mazeFile() {
    return m_mazeFile;
}

bool Param::useMazeFile() {
    return m_useMazeFile;
}

double Param::wallWidth() {
    return m_wallWidth;
}

double Param::wallLength() {
    return m_wallLength;
}

int Param::generatedMazeWidth() {
    return m_generatedMazeWidth;
}

int Param::generatedMazeHeight() {
    return m_generatedMazeHeight;
}

std::string Param::mazeAlgorithm() {
    return m_mazeAlgorithm;
}

bool Param::saveGeneratedMaze() {
    return m_saveGeneratedMaze;
}

std::string Param::generatedMazeFile() {
    return m_generatedMazeFile;
}

std::string Param::generatedMazeType() {
    return m_generatedMazeType;
}

bool Param::mazeMirrored() {
    return m_mazeMirrored;
}

int Param::mazeRotations() {
    return m_mazeRotations;
}

std::string Param::mouseAlgorithm() {
    return m_mouseAlgorithm;
}

} // namespace sim
