#include "Param.h"

#include <limits>
#include <random>

#include "Color.h"
#include "Direction.h"
#include "Directory.h"
#include "Layout.h"
#include "Logging.h"
#include "ParamParser.h"

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
        "default-window-width", 930, 100);
    m_defaultWindowHeight = parser.getIntIfHasIntAndNotLessThan(
        "default-window-height", 470, 100);
    m_defaultLayout = parser.getStringIfHasStringAndIsLayout(
        "default-layout", LAYOUT_TO_STRING.at(Layout::BOTH));
    m_windowBorderWidth = parser.getIntIfHasIntAndInRange(
        "window-border-width", 10, 0, 25);
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
    m_mouseWheelColor = parser.getStringIfHasStringAndIsColor(
        "mouse-wheel-color", COLOR_TO_STRING.at(Color::GREEN));
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
    m_tileFogAlpha = parser.getDoubleIfHasDoubleAndInRange(
        "tile-fog-alpha", 0.15, 0.0, 1.0);
    m_defaultWireframeMode = parser.getBoolIfHasBool(
        "default-wireframe-mode", false);
    m_setTileBaseColorWhenDistanceCorrect = parser.getBoolIfHasBool(
        "set-tile-base-color-when-distance-correct", false);
    m_distanceCorrectTileBaseColor = parser.getStringIfHasStringAndIsColor(
        "distance-correct-tile-base-color", COLOR_TO_STRING.at(Color::DARK_YELLOW));

    // Simulation Parameters
    bool useRandomSeed = parser.getBoolIfHasBool(
        "use-random-seed", false);
    if (useRandomSeed && !parser.hasIntValue("random-seed")) {
#ifndef _WIN32 // TODO: MACK
        Logging::simLogger()->warn(
            "The value of use-random-seed is true but no valid random-seed "
            "value was provided. Setting \"use-random-seed\" to false.");
#endif
        useRandomSeed = false;
    }
    m_randomSeed = (useRandomSeed ? parser.getIntValue("random-seed") : std::random_device()());
    m_crashMessage = parser.getStringIfHasString(
        "crash-message", "CRASH");
    m_glutInitDuration = parser.getDoubleIfHasDoubleAndInRange(
        "glut-init-duration", 0.1, 0.0, 5.0);
    m_defaultPaused = parser.getBoolIfHasBool(
        "default-paused", false);
    m_minSleepDuration = parser.getDoubleIfHasDoubleAndInRange(
        "min-sleep-duration", 5, 1, 25);

    // TODO: MACK - get rid of these once I've written the simulation time component
    m_discreteInterfaceMinSpeed = parser.getDoubleIfHasDouble(
        "discrete-interface-min-speed", 1.0);
    m_discreteInterfaceMaxSpeed = parser.getDoubleIfHasDouble(
        "discrete-interface-max-speed", 300.0);
    m_discreteInterfaceDefaultSpeed = parser.getDoubleIfHasDoubleAndInRange(
        "discrete-interface-default-speed", 30.0, m_discreteInterfaceMinSpeed, m_discreteInterfaceMaxSpeed);

    m_discreteInterfaceDeclareWallOnRead = parser.getBoolIfHasBool(
        "discrete-interface-declare-wall-on-read", true);
    m_algorithmControlsTileFog = parser.getBoolIfHasBool(
        "algorithm-controls-tile-fog", false);
    m_declareBothWallHalves = parser.getBoolIfHasBool(
        "declare-both-wall-halves", true);
    m_mousePositionUpdateRate = parser.getIntIfHasIntAndInRange(
        "mouse-position-update-rate", 1000, 100, 10000);
    m_printLateMousePostitionUpdates = parser.getBoolIfHasBool(
        "print-late-mouse-position-updates", false);
    m_collisionDetectionRate = parser.getIntIfHasIntAndInRange(
        "collision-detection-rate", 40, 1, 100);
    m_printLateCollisionDetections = parser.getBoolIfHasBool(
        "print-late-collision-detections", false);
    m_printLateSensorReads = parser.getBoolIfHasBool(
        "print-late-sensor-reads", false);
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
    m_wallHeight = parser.getDoubleIfHasDoubleAndInRange(
        "wall-height", 0.05, 0.025, 0.1);
    m_enforceOfficialMazeRules = parser.getBoolIfHasBool(
        "enforce-official-maze-rules", false);
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
    m_mazeMirrored = parser.getBoolIfHasBool(
        "maze-mirrored", false);
    m_mazeRotations = parser.getIntIfHasIntAndInRange(
        "maze-rotations", 0, 0, 3);

    // Mouse parameters
    m_mouseAlgorithm = parser.getStringIfHasString(
        "mouse-algorithm", "RightWallFollow");
    m_mouseStartingDirection = parser.getStringIfHasStringAndIsDirection(
        "mouse-starting-direction", DIRECTION_TO_STRING.at(Direction::NORTH));
}

int Param::defaultWindowWidth() {
    return m_defaultWindowWidth;
}

int Param::defaultWindowHeight() {
    return m_defaultWindowHeight;
}

std::string Param::defaultLayout() {
    return m_defaultLayout;
}

int Param::windowBorderWidth() {
    return m_windowBorderWidth;
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

std::string Param::mouseWheelColor() {
    return m_mouseWheelColor;
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

double Param::tileFogAlpha() {
    return m_tileFogAlpha;
}

bool Param::defaultWireframeMode() {
    return m_defaultWireframeMode;
}

bool Param::setTileBaseColorWhenDistanceCorrect() {
    return m_setTileBaseColorWhenDistanceCorrect;
}

std::string Param::distanceCorrectTileBaseColor() {
    return m_distanceCorrectTileBaseColor;
}

int Param::randomSeed() {
    return m_randomSeed;
}

std::string Param::crashMessage() {
    return m_crashMessage;
}

double Param::glutInitDuration() {
    return m_glutInitDuration;
}

bool Param::defaultPaused() {
    return m_defaultPaused;
}

double Param::minSleepDuration() {
    return m_minSleepDuration;
}

double Param::discreteInterfaceMinSpeed() {
    return m_discreteInterfaceMinSpeed;
}

double Param::discreteInterfaceMaxSpeed() {
    return m_discreteInterfaceMaxSpeed;
}

double Param::discreteInterfaceDefaultSpeed() {
    return m_discreteInterfaceDefaultSpeed;
}

bool Param::discreteInterfaceDeclareWallOnRead() {
    return m_discreteInterfaceDeclareWallOnRead;
}

bool Param::algorithmControlsTileFog() {
    return m_algorithmControlsTileFog;
}

bool Param::declareBothWallHalves() {
    return m_declareBothWallHalves;
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

bool Param::printLateSensorReads() {
    return m_printLateSensorReads;
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

double Param::wallHeight() {
    return m_wallHeight;
}

int Param::generatedMazeWidth() {
    return m_generatedMazeWidth;
}

int Param::generatedMazeHeight() {
    return m_generatedMazeHeight;
}

bool Param::enforceOfficialMazeRules() {
    return m_enforceOfficialMazeRules;
}

std::string Param::mazeAlgorithm() {
    return m_mazeAlgorithm;
}

bool Param::saveGeneratedMaze() {
    return m_saveGeneratedMaze;
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

std::string Param::mouseStartingDirection() {
    return m_mouseStartingDirection;
}

} // namespace sim
