#include "Param.h"

#include <iostream>

#include "../lib/pugixml.hpp"

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

    // Always give all of the parameters default values
    m_PIXELS_PER_UNIT = 4;
    m_UNITS_PER_TILE = 9;
    m_SLEEP_TIME_MIN = 5;
    m_SLEEP_TIME_MAX = 1500;
    m_MAZE_FILE = "";
    m_MAZE_WIDTH = 16;
    m_MAZE_HEIGHT = 16;
    m_MIN_MAZE_STEPS = 40;
    m_MAX_DISTANCE = m_MAZE_WIDTH * m_MAZE_HEIGHT;
    m_WINDOW_WIDTH = m_MAZE_WIDTH * m_UNITS_PER_TILE * m_PIXELS_PER_UNIT;
    m_WINDOW_HEIGHT =  m_MAZE_HEIGHT * m_UNITS_PER_TILE * m_PIXELS_PER_UNIT;

    // Override with user-supplied values if possible
    pugi::xml_document doc;
    std::string parameterFile = "parameters.xml";
    pugi::xml_parse_result result = doc.load_file(parameterFile.c_str());
    if (!result) {
        std::cout << "Unable to read parameters from \"" << parameterFile << "\" : "
                  << result.description() << std::endl;
        return;
    }

    // Graphical Parameters
    if (!std::string(doc.child("pixels-per-unit").child_value()).empty()) {
        m_PIXELS_PER_UNIT = atof(doc.child("pixels-per-unit").child_value());
    }
    if (!std::string(doc.child("units-per-tile").child_value()).empty()) {
        m_UNITS_PER_TILE = atof(doc.child("units-per-tile").child_value());
    }

    // Simulation Parameters
    if (!std::string(doc.child("min-sim-speed").child_value()).empty()) {
        m_SLEEP_TIME_MIN = atof(doc.child("min-sim-speed").child_value());
    }
    if (!std::string(doc.child("max-sim-speed").child_value()).empty()) {
        m_SLEEP_TIME_MAX = atof(doc.child("max-sim-speed").child_value());
    }

    // Maze Parameters
    if (!std::string(doc.child("maze-file").child_value()).empty()) {
        m_MAZE_FILE = doc.child("maze-file").child_value();
    }
    if (!std::string(doc.child("random-maze-width").child_value()).empty()) {
        m_MAZE_WIDTH = atof(doc.child("random-maze-width").child_value());
    }
    if (!std::string(doc.child("random-maze-height").child_value()).empty()) {
        m_MAZE_HEIGHT = atof(doc.child("random-maze-height").child_value());
    }
    if (!std::string(doc.child("min-solution-length").child_value()).empty()) {
        m_MIN_MAZE_STEPS = atof(doc.child("min-solution-length").child_value());
    }

    // Update the non-configurable parameters
    m_MAX_DISTANCE = m_MAZE_WIDTH * m_MAZE_HEIGHT;
    m_WINDOW_WIDTH = m_MAZE_WIDTH * m_UNITS_PER_TILE * m_PIXELS_PER_UNIT;
    m_WINDOW_HEIGHT =  m_MAZE_HEIGHT * m_UNITS_PER_TILE * m_PIXELS_PER_UNIT;
}

int Param::SLEEP_TIME_MIN() {
    return m_SLEEP_TIME_MIN;
}

int Param::SLEEP_TIME_MAX() {
    return m_SLEEP_TIME_MAX;
}

int Param::MAZE_WIDTH() {
    return m_MAZE_WIDTH;
}

int Param::MAZE_HEIGHT() {
    return m_MAZE_HEIGHT;
}

int Param::MAX_DISTANCE() {
    return m_MAX_DISTANCE;
}

int Param::MIN_MAZE_STEPS() {
    return m_MIN_MAZE_STEPS;
}

std::string Param::MAZE_FILE() {
    return m_MAZE_FILE;
}

int Param::PIXELS_PER_UNIT() {
    return m_PIXELS_PER_UNIT;
}

int Param::UNITS_PER_TILE() {
    return m_UNITS_PER_TILE;
}

int Param::WINDOW_WIDTH() {
    return m_WINDOW_WIDTH;
}

int Param::WINDOW_HEIGHT() {
    return m_WINDOW_HEIGHT;
}

} // namespace sim
