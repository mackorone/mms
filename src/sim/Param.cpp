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

    // Always give the parameters default values
    m_SLEEP_TIME_MIN = 5;
    m_SLEEP_TIME_MAX = 1500;
    m_MAZE_WIDTH = 16;
    m_MAZE_HEIGHT = 16;
    m_MAZE_FILE = "";
    m_MIN_MAZE_STEPS = 40;
    m_PIXELS_PER_UNIT = 4;
    m_UNITS_PER_TILE = 9;

    // Override them with user-supplies values if we can
    pugi::xml_document doc;
    if (!doc.load_file("Parameters.xml")) {
        std::cout << "Unable to find \"Parameters.xml\". Using default values..." << std::endl;
        return;
    }

    if (!std::string(doc.child("SLEEP_TIME_MIN").child_value()).empty()) {
        m_SLEEP_TIME_MIN = atof(doc.child("SLEEP_TIME_MIN").child_value());
    }
    if (!std::string(doc.child("SLEEP_TIME_MAX").child_value()).empty()) {
        m_SLEEP_TIME_MAX = atof(doc.child("SLEEP_TIME_MAX").child_value());
    }
    if (!std::string(doc.child("MAZE_WIDTH").child_value()).empty()) {
        m_MAZE_WIDTH = atof(doc.child("MAZE_WIDTH").child_value());
    }
    if (!std::string(doc.child("MAZE_HEIGHT").child_value()).empty()) {
        m_MAZE_HEIGHT = atof(doc.child("MAZE_HEIGHT").child_value());
    }
    if (!std::string(doc.child("MAZE_FILE").child_value()).empty()) {
        m_MAZE_FILE = doc.child("MAZE_FILE").child_value();
    }
    if (!std::string(doc.child("MIN_MAZE_STEPS").child_value()).empty()) {
        m_MIN_MAZE_STEPS = atof(doc.child("MIN_MAZE_STEPS").child_value());
    }
    if (!std::string(doc.child("PIXELS_PER_UNIT").child_value()).empty()) {
        m_PIXELS_PER_UNIT = atof(doc.child("PIXELS_PER_UNIT").child_value());
    }
    if (!std::string(doc.child("UNITS_PER_TILE").child_value()).empty()) {
        m_UNITS_PER_TILE = atof(doc.child("UNITS_PER_TILE").child_value());
    }

    // Non-configurable parameters
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
