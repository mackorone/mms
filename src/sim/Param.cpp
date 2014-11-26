#include "Param.h"

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

    // TODO: Look for Parameter.xml... if can't find it, make a note and default initialize to certain values

    m_SLEEP_TIME_MIN = 5;
    m_SLEEP_TIME_MAX = 1500;
    m_MAZE_WIDTH = 16;
    m_MAZE_HEIGHT = 16;
    m_MAX_DISTANCE = m_MAZE_WIDTH * m_MAZE_HEIGHT;
    m_MAZE_FILE = "maze2011.maz";
    m_MIN_MAZE_STEPS = 40;

    m_PIXELS_PER_UNIT = 4;
    m_UNITS_PER_TILE = 9;
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
