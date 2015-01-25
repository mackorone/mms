#include "State.h"

#include <string>

namespace sim {

State* S() {
    return State::getInstance();
}

// Definition of the variable for linking
State* State::INSTANCE = NULL;
State* State::getInstance() {
    if (NULL == INSTANCE) {
        INSTANCE = new State();
    }
    return INSTANCE;
}

State::State() {

    m_paused = false;
    m_simSpeed = 50;
    m_undoRequested = false;
    m_resetRequested = false;
}

int State::simSpeed() {
    return m_simSpeed;
}
bool State::paused() {
    return m_paused;
}
bool State::undoRequested() {
    return m_undoRequested;
}
bool State::resetRequested() {
    return m_resetRequested;
}
void State::setSimSpeed(int simSpeed) {
    m_simSpeed = simSpeed;
}
void State::setPaused(bool paused) {
    m_paused = paused;
}
void State::setUndoRequested(bool undoRequested) {
    m_undoRequested = undoRequested;
}
void State::setResetRequested(bool resetRequested) {
    m_resetRequested = resetRequested;
}

} // namespace sim
