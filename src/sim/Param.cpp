#include "Param.h"

#include <iostream>

#include "../lib/pugixml.hpp" // TODO: can we automaticlaly include lib directory in the build...
#include "Utilities.h"

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

    // Initialization list??? // TODO
    m_mazeWidth = 16;
    m_mazeHeight = 16;

    m_wallWidth = 0.012;
    m_wallLength = 0.156;
    m_wallHeight = 0.05;
    m_wallTolerance = 0.05;

    // Always give all of the parameters default values
    m_pixelsPerMeter = 250;
    m_borderSize = 10;
    m_frameRate = 60;
    m_minSimSpeed = 1; // Percent of real-time
    m_maxSimSpeed = 10000; // Percent of real-time
    m_mazeFile = "";
    m_minSolutionLength = 40;
    m_windowWidth = (m_mazeWidth * (m_wallLength + m_wallWidth) + m_wallWidth) * m_pixelsPerMeter;
    m_windowHeight = (m_mazeHeight * (m_wallLength + m_wallWidth) + m_wallWidth) * m_pixelsPerMeter;

    // Override with user-supplied values if possible
    pugi::xml_document doc;
    std::string parameterFile = getProjectDirectory() + "src/sim/parameters.xml";
    pugi::xml_parse_result result = doc.load_file(parameterFile.c_str());
    if (!result) {
        std::cout << "Unable to read parameters from \"" << parameterFile << "\" : "
                  << result.description() << std::endl;
        return;
    }

    // TODO: Make library independent, type checked version of the XML parser

    // TODO: Ensure "TYPE" of values is correct
    // TODO: Should we check to see if it's empty??

    // Graphical Parameters
    if (!std::string(doc.child("pixels-per-meter").child_value()).empty()) {
        m_pixelsPerMeter = atof(doc.child("pixels-per-meter").child_value());
    }
    if (!std::string(doc.child("frame-rate").child_value()).empty()) {
        m_frameRate = atof(doc.child("frame-rate").child_value());
    }

    // Simulation Parameters
    if (!std::string(doc.child("min-sim-speed").child_value()).empty()) {
        m_minSimSpeed = atof(doc.child("min-sim-speed").child_value());
    }
    if (!std::string(doc.child("max-sim-speed").child_value()).empty()) {
        m_maxSimSpeed = atof(doc.child("max-sim-speed").child_value());
    }

    // Maze Parameters
    if (!std::string(doc.child("maze-directory").child_value()).empty()) {
        m_mazeDirectory = doc.child("maze-directory").child_value();
    }
    if (!std::string(doc.child("maze-file").child_value()).empty()) {
        m_mazeFile = doc.child("maze-file").child_value();
    }
    if (!std::string(doc.child("random-maze-width").child_value()).empty()) {
        m_mazeWidth = atof(doc.child("random-maze-width").child_value()); // TODO read from file
    }
    if (!std::string(doc.child("random-maze-height").child_value()).empty()) {
        m_mazeHeight = atof(doc.child("random-maze-height").child_value()); // TODO read from file
    }
    if (!std::string(doc.child("min-solution-length").child_value()).empty()) {
        m_minSolutionLength = atof(doc.child("min-solution-length").child_value());
    }

    // Update the non-configurable parameters
    m_windowWidth = (m_mazeWidth * (m_wallLength + m_wallWidth)) * m_pixelsPerMeter;
    m_windowHeight = (m_mazeHeight * (m_wallLength + m_wallWidth)) * m_pixelsPerMeter;

    // TODO: This should be in the randomization code
    // Ensure that the size parameters are valid
    ASSERT(m_mazeWidth > 0); // TODO
    ASSERT(m_mazeHeight > 0); // TODO
}

int Param::minSimSpeed() {
    return m_minSimSpeed;
}

int Param::maxSimSpeed() {
    return m_maxSimSpeed;
}

int Param::minSolutionLength() {
    return m_minSolutionLength;
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

int Param::pixelsPerMeter() {
    return m_pixelsPerMeter;
}

int Param::borderSize() {
    return m_borderSize;
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

} // namespace sim
