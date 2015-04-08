#include "State.h"

#include "Assert.h"
#include "Param.h"
#include "SimUtilities.h"

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
    m_crashed = false;
    m_interfaceType = UNDECLARED;
    m_mazeVisible = P()->defaultMazeVisible();
    m_mouseVisible = P()->defaultMouseVisible();
    m_mousePathVisible = P()->defaultMousePathVisible();
    m_wallTruthVisible = P()->defaultWallTruthVisible();
    m_tileColorsVisible = P()->defaultTileColorsVisible();
    m_paused = P()->defaultPaused();
    m_simSpeed = P()->discreteInterfaceDefaultSpeed();
    for (int i = 0; i < 10; i += 1) {
        m_inputButtons[i] = false;
    }
}

bool State::crashed() {
    return m_crashed;
}

InterfaceType State::interfaceType() {
    return m_interfaceType;
}

bool State::mazeVisible() {
    return m_mazeVisible;
}

bool State::mouseVisible() {
    return m_mouseVisible;
}

bool State::mousePathVisible() {
    return m_mousePathVisible;
}

bool State::wallTruthVisible() {
    return m_wallTruthVisible;
}

bool State::tileColorsVisible() {
    return m_tileColorsVisible;
}

bool State::paused() {
    return m_paused;
}

float State::simSpeed() {
    return m_simSpeed;
}

bool State::inputButtonPressed(int inputButton) {
    ASSERT(0 <= inputButton && inputButton <= 9);
    return m_inputButtons.at(inputButton);
}

void State::setCrashed() {
    m_crashed = true;
    print(P()->crashMessage());
}

void State::setInterfaceType(InterfaceType interfaceType) {
    ASSERT(m_interfaceType == UNDECLARED);
    m_interfaceType = interfaceType;
}

void State::setMazeVisible(bool mazeVisible) {
    m_mazeVisible = mazeVisible;
}

void State::setMouseVisible(bool mouseVisible) {
    m_mouseVisible = mouseVisible;
}

void State::setMousePathVisible(bool mousePathVisible) {
    m_mousePathVisible = mousePathVisible;
}

void State::setWallTruthVisible(bool wallTruthVisible) {
    m_wallTruthVisible = wallTruthVisible;
}

void State::setTileColorsVisible(bool tileColorsVisible) {
    m_tileColorsVisible = tileColorsVisible;
}

void State::setPaused(bool paused) {
    m_paused = paused;
}

void State::setSimSpeed(float simSpeed) {
    if (simSpeed < P()->discreteInterfaceMinSpeed()) {
        m_simSpeed = P()->discreteInterfaceMinSpeed();
    }
    else if (P()->discreteInterfaceMaxSpeed() < simSpeed) {
        m_simSpeed = P()->discreteInterfaceMaxSpeed();
    }
    else {
        m_simSpeed = simSpeed;
    }
}

void State::setInputButtonPressed(int inputButton, bool pressed) {
    ASSERT(0 <= inputButton && inputButton <= 9);
    m_inputButtons[inputButton] = pressed;
}

} // namespace sim
