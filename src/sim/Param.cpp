#include "Param.h"

#include "ParamParser.h"
#include "SimUtilities.h"

namespace sim {

Param* P() {
    return Param::getInstance();
}

// Definition of the variable for linking
Param* Param::INSTANCE = NULL;
Param* Param::getInstance() {
    if (NULL == INSTANCE) {
        INSTANCE = new Param();
    }
    return INSTANCE;
}

Param::Param() {

    // Create the parameter parser object
    ParamParser parser(SimUtilities::getProjectDirectory() + "res/parameters.xml");

    // Graphical Parameters
    m_pixelsPerMeter = parser.getIntIfHasInt("pixels-per-meter", 250);
    m_frameRate = parser.getIntIfHasInt("frame-rate", 60);
    m_printLateFrames = parser.getBoolIfHasBool("print-late-frames", false);
    m_tileBaseColor = parser.getStringIfHasString("tile-base-color", "BLACK");
    m_tileWallColor = parser.getStringIfHasString("tile-wall-color", "RED");
    m_tileCornerColor = parser.getStringIfHasString("tile-corner-color", "GRAY");
    m_tileTextColor = parser.getStringIfHasString("tile-text-color", "GRAY");
    m_tileUndeclaredWallColor = parser.getStringIfHasString("tile-undeclared-wall-color", "DARK_RED");
    m_tileUndeclaredNoWallColor = parser.getStringIfHasString("tile-undeclared-no-wall-color", "DARK_GRAY");
    m_mouseBodyColor = parser.getStringIfHasString("mouse-body-color", "BLUE");
    m_mouseWheelColor = parser.getStringIfHasString("mouse-wheel-color", "GREEN");
    m_mouseSensorColor = parser.getStringIfHasString("mouse-sensor-color", "WHITE");
    m_defaultMazeVisible = parser.getBoolIfHasBool("default-maze-visible", true);
    m_defaultMouseVisible = parser.getBoolIfHasBool("default-mouse-visible", true);
    m_defaultMousePathVisible = parser.getBoolIfHasBool("default-mouse-path-visible", true);
    m_defaultWallTruthVisible = parser.getBoolIfHasBool("default-wall-truth-visible", false);
    m_defaultTileColorsVisible = parser.getBoolIfHasBool("default-tile-colors-visible", true);

    // Simulation Parameters
    bool useRandomSeed = parser.getBoolIfHasBool("use-random-seed", false);
    m_randomSeed = (useRandomSeed ? parser.getIntIfHasInt("random-seed", time(NULL)) : time(NULL));
    m_crashMessage = parser.getStringIfHasString("crash-message", "CRASH");
    m_glutInitDuration = parser.getFloatIfHasFloat("glut-init-duration", 0.25);
    m_defaultPaused = parser.getBoolIfHasBool("default-paused", false);
    m_discreteInterfaceSleepDuration = parser.getFloatIfHasFloat("discrete-interface-sleep-duration", 10);
    m_discreteInterfaceMinSpeed = parser.getFloatIfHasFloat("discrete-interface-min-speed", 1.0);
    m_discreteInterfaceMaxSpeed = parser.getFloatIfHasFloat("discrete-interface-max-speed", 500.0);
    m_discreteInterfaceDefaultSpeed = parser.getFloatIfHasFloat("discrete-interface-default-speed", 30.0);
    m_discreteInterfaceDeclareWallOnRead = parser.getBoolIfHasBool("discrete-interface-declare-wall-on-read", true);
    m_declareBothWallHalves = parser.getBoolIfHasBool("declare-both-wall-halves", true);
    m_mousePositionUpdateRate = parser.getIntIfHasInt("mouse-position-update-rate", 1000);
    m_printLateMousePostitionUpdates = parser.getBoolIfHasBool("print-late-mouse-position-updates", false);
    m_collisionDetectionRate = parser.getIntIfHasInt("collision-detection-rate", 40);
    m_printLateCollisionDetections = parser.getBoolIfHasBool("print-late-collision-detections", false);
    m_printLateSensorReads = parser.getBoolIfHasBool("print-late-sensor-reads", false);
    m_numberOfCircleApproximationPoints = parser.getFloatIfHasFloat("number-of-circle-approximation-points", 8);
    m_numberOfSensorEdgePoints = parser.getFloatIfHasFloat("number-of-sensor-edge-points", 3);

    // Maze Parameters
    m_mazeDirectory = parser.getStringIfHasString("maze-directory", "res/mazes/");
    m_mazeFile = parser.getStringIfHasString("maze-file", "");
    m_useMazeFile = parser.getBoolIfHasBool("use-maze-file", false);
    m_wallWidth = parser.getFloatIfHasFloat("wall-width", 0.012);
    m_wallLength = parser.getFloatIfHasFloat("wall-length", 0.156);
    m_wallHeight = parser.getFloatIfHasFloat("wall-height", 0.05);
    m_generatedMazeWidth = parser.getIntIfHasInt("generated-maze-width", 16);
    m_generatedMazeHeight = parser.getIntIfHasInt("generated-maze-height", 16);
    m_enforceOfficialMazeRules = parser.getBoolIfHasBool("enforce-official-maze-rules", true);
    m_mazeGenerationAlgo = parser.getStringIfHasString("maze-generation-algo", "TOMASZ");
    m_saveGeneratedMaze = parser.getBoolIfHasBool("save-generated-maze", true);

    // Mouse parameters
    m_mouseDirectory = parser.getStringIfHasString("mouse-directory", "res/mice/");

    // Algorithm parameters
    m_algorithm = parser.getStringIfHasString("algorithm", "MackAlgo");
}

int Param::pixelsPerMeter() {
    return m_pixelsPerMeter;
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

std::string Param::tileTextColor() {
    return m_tileTextColor;
}

std::string Param::tileUndeclaredWallColor() {
    return m_tileUndeclaredWallColor;
}

std::string Param::tileUndeclaredNoWallColor() {
    return m_tileUndeclaredNoWallColor;
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

bool Param::defaultMazeVisible() {
    return m_defaultMazeVisible;
}

bool Param::defaultMouseVisible() {
    return m_defaultMouseVisible;
}

bool Param::defaultMousePathVisible() {
    return m_defaultMousePathVisible;
}

bool Param::defaultWallTruthVisible() {
    return m_defaultWallTruthVisible;
}

bool Param::defaultTileColorsVisible() {
    return m_defaultTileColorsVisible;
}

int Param::randomSeed() {
    return m_randomSeed;
}

std::string Param::crashMessage() {
    return m_crashMessage;
}

float Param::glutInitDuration() {
    return m_glutInitDuration;
}

bool Param::defaultPaused() {
    return m_defaultPaused;
}

float Param::discreteInterfaceSleepDuration() {
    return m_discreteInterfaceSleepDuration;
}

float Param::discreteInterfaceMinSpeed() {
    return m_discreteInterfaceMinSpeed;
}

float Param::discreteInterfaceMaxSpeed() {
    return m_discreteInterfaceMaxSpeed;
}

float Param::discreteInterfaceDefaultSpeed() {
    return m_discreteInterfaceDefaultSpeed;
}

bool Param::discreteInterfaceDeclareWallOnRead() {
    return m_discreteInterfaceDeclareWallOnRead;
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

std::string Param::mazeDirectory() {
    return m_mazeDirectory;
}

std::string Param::mazeFile() {
    return m_mazeFile;
}

bool Param::useMazeFile() {
    return m_useMazeFile;
}

float Param::wallWidth() {
    return m_wallWidth;
}

float Param::wallLength() {
    return m_wallLength;
}

float Param::wallHeight() {
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

std::string Param::mazeGenerationAlgo() {
    return m_mazeGenerationAlgo;
}

bool Param::saveGeneratedMaze() {
    return m_saveGeneratedMaze;
}

std::string Param::mouseDirectory() {
    return m_mouseDirectory;
}

std::string Param::algorithm() {
    return m_algorithm;
}

} // namespace sim
