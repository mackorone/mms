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
    ParamParser parser(getProjectDirectory() + "res/parameters.xml");

    // Graphical Parameters
    m_pixelsPerMeter = parser.getIntIfHasInt("pixels-per-meter", 250);
    m_frameRate = parser.getIntIfHasInt("frame-rate", 60);
    m_tileBaseColor = parser.getStringIfHasString("tile-base-color", "BLACK");
    m_tileWallColor = parser.getStringIfHasString("tile-wall-color", "RED");
    m_tileCornerColor = parser.getStringIfHasString("tile-corner-color", "GRAY");
    m_mouseBodyColor = parser.getStringIfHasString("mouse-body-color", "BLUE");
    m_mouseWheelColor = parser.getStringIfHasString("mouse-wheel-color", "GREEN");
    m_mouseSensorColor = parser.getStringIfHasString("mouse-sensor-color", "WHITE");

    // Simulation Parameters
    bool useRandomSeed = parser.getBoolIfHasBool("use-random-seed", false); // TODO: Is this correct behavior???
    m_randomSeed = (useRandomSeed ? parser.getIntIfHasInt("random-seed", time(NULL)) : time(NULL));
    m_glutInitTime = parser.getFloatIfHasFloat("glut-init-time", 0.25);
    m_deltaTime = parser.getFloatIfHasFloat("delta-time", 0.01);
    m_minSimSpeed = parser.getFloatIfHasFloat("min-sim-speed", 0.1);
    m_maxSimSpeed = parser.getFloatIfHasFloat("max-sim-speed", 10);
    m_defaultSimSpeed = parser.getFloatIfHasFloat("default-sim-speed", 10);
    m_numberOfCircleApproximationPoints = parser.getFloatIfHasFloat("number-of-circle-approximation-points", 8);
    m_numberOfSensorEdgePoints = parser.getFloatIfHasFloat("number-of-sensor-edge-points", 10);

    // Maze Parameters
    m_mazeDirectory = parser.getStringIfHasString("maze-directory", "src/mazes/");
    m_mazeFile = parser.getStringIfHasString("maze-file", "");
    m_useMazeFile = parser.getBoolIfHasBool("use-maze-file", false);
    m_randomMazeWidth = parser.getIntIfHasInt("random-maze-width", 16);
    m_randomMazeHeight = parser.getIntIfHasInt("random-maze-height", 16);
    m_wallWidth = parser.getFloatIfHasFloat("wall-width", 0.012);
    m_wallLength = parser.getFloatIfHasFloat("wall-length", 0.156);
    m_wallHeight = parser.getFloatIfHasFloat("wall-height", 0.05);
    m_enforceOfficialMazeRules = parser.getBoolIfHasBool("enforce-official-maze-rules", true);
    m_randomMazeAlgo = parser.getStringIfHasString("random-maze-algo", "TOMASZ");
    m_saveRandomMaze = parser.getBoolIfHasBool("save-random-maze", true);

    // Update the non-configurable parameters
    m_windowWidth = (m_randomMazeWidth * (m_wallLength + m_wallWidth)) * m_pixelsPerMeter;
    m_windowHeight = (m_randomMazeHeight * (m_wallLength + m_wallWidth)) * m_pixelsPerMeter;
}

int Param::pixelsPerMeter() {
    return m_pixelsPerMeter;
}

int Param::frameRate() {
    return m_frameRate;
}

int Param::windowWidth() {
    return m_windowWidth;
}

int Param::windowHeight() {
    return m_windowHeight;
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

std::string Param::mouseBodyColor() {
    return m_mouseBodyColor;
}

std::string Param::mouseWheelColor() {
    return m_mouseWheelColor;
}

std::string Param::mouseSensorColor() {
    return m_mouseSensorColor;
}

int Param::randomSeed() {
    return m_randomSeed;
}

float Param::glutInitTime() {
    return m_glutInitTime;
}

float Param::deltaTime() {
    return m_deltaTime;
}

float Param::minSimSpeed() {
    return m_minSimSpeed;
}

float Param::maxSimSpeed() {
    return m_maxSimSpeed;
}

float Param::defaultSimSpeed() {
    return m_defaultSimSpeed;
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

int Param::randomMazeWidth() {
    return m_randomMazeWidth;
}

int Param::randomMazeHeight() {
    return m_randomMazeHeight;
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

bool Param::enforceOfficialMazeRules() {
    return m_enforceOfficialMazeRules;
}

std::string Param::randomMazeAlgo() {
    return m_randomMazeAlgo;
}

bool Param::saveRandomMaze() {
    return m_saveRandomMaze;
}

} // namespace sim
