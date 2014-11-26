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

    m_PAUSED = false;
    m_SLEEP_TIME = 150;
    m_UNDO_REQUESTED = false;
    m_RESET_REQUESTED = false;
}

int State::SLEEP_TIME() {
    return m_SLEEP_TIME;
}
bool State::PAUSED() {
    return m_PAUSED;
}
bool State::UNDO_REQUESTED() {
    return m_UNDO_REQUESTED;
}
bool State::RESET_REQUESTED() {
    return m_RESET_REQUESTED;
}
void State::SET_SLEEP_TIME(int sleep_time) {
    m_SLEEP_TIME = sleep_time;
}
void State::SET_PAUSED(bool paused) {
    m_PAUSED = paused;
}
void State::SET_UNDO_REQUESTED(bool undo_requested) {
    m_UNDO_REQUESTED = undo_requested;
}
void State::SET_RESET_REQUESTED(bool reset_requested) {
    m_RESET_REQUESTED = reset_requested;
}

} // namespace sim
