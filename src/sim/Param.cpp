#include "Param.h"

#include "ParamParser.h"
#include "Utilities.h" // TODO: Take this out once we take out the ASSERT

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
    ParamParser parser(getProjectDirectory() + "src/sim/parameters.xml"); // TODO: Configure this???

    // Graphical Parameters
    m_pixelsPerMeter = parser.getIntIfHasInt("pixels-per-meter", 250);
    m_frameRate = parser.getIntIfHasInt("frame-rate", 60);

    // Simulation Parameters
    m_minSimSpeed = parser.getFloatIfHasFloat("min-sim-speed", 0.1);
    m_maxSimSpeed = parser.getFloatIfHasFloat("max-sim-speed", 10);

    // Maze Parameters
    m_mazeDirectory = parser.getStringIfHasString("maze-directory", "src/maze_files/");
    m_mazeFile = parser.getStringIfHasString("maze-file", "");
    m_mazeWidth = parser.getIntIfHasInt("random-maze-width", 16);
    m_mazeHeight = parser.getIntIfHasInt("random-maze-height", 16);
    m_wallWidth = parser.getFloatIfHasFloat("wall-width", 0.012);
    m_wallLength = parser.getFloatIfHasFloat("wall-length", 0.156);
    m_wallHeight = parser.getFloatIfHasFloat("wall-height", 0.05);
    m_wallTolerance = parser.getFloatIfHasFloat("wall-tolerance", 0.05);
    m_minSolutionLength = parser.getIntIfHasInt("min-solution-length", 40);
    m_saveRandomMaze = parser.getBoolIfHasBool("save-random-maze", true);

    // Update the non-configurable parameters
    m_windowWidth = (m_mazeWidth * (m_wallLength + m_wallWidth)) * m_pixelsPerMeter;
    m_windowHeight = (m_mazeHeight * (m_wallLength + m_wallWidth)) * m_pixelsPerMeter;

    // TODO: This should be in the randomization code
    // Ensure that the size parameters are valid
    ASSERT(m_mazeWidth > 0); // TODO
    ASSERT(m_mazeHeight > 0); // TODO
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

int Param::minSimSpeed() {
    return m_minSimSpeed;
}

int Param::maxSimSpeed() {
    return m_maxSimSpeed;
}

std::string Param::mazeDirectory() {
    return m_mazeDirectory;
}

std::string Param::mazeFile() {
    return m_mazeFile;
}

int Param::mazeWidth() {
    return m_mazeWidth;
}

int Param::mazeHeight() {
    return m_mazeHeight;
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

float Param::wallTolerance() {
    return m_wallTolerance;
}

int Param::minSolutionLength() {
    return m_minSolutionLength;
}

bool Param::saveRandomMaze() {
    return m_saveRandomMaze;
}

} // namespace sim
