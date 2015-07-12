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
    m_mainThreadId = std::this_thread::get_id();
    m_crashed = false;
    m_interfaceType = UNDECLARED;
    m_rotateZoomedMap = P()->defaultRotateZoomedMap();
    m_zoomedMapScale = P()->defaultZoomedMapScale();
    m_mousePathVisible = P()->defaultMousePathVisible();
    m_wallTruthVisible = P()->defaultWallTruthVisible();
    m_tileColorsVisible = P()->defaultTileColorsVisible();
    m_tileTextVisible = P()->defaultTileTextVisible();
    m_tileFogVisible = P()->defaultTileFogVisible();
    m_wireframeMode = P()->defaultWireframeMode();
    m_paused = P()->defaultPaused();
    m_simSpeed = P()->discreteInterfaceDefaultSpeed();
    for (int i = 0; i < 10; i += 1) {
        m_inputButtons[i] = false;
    }
}

std::thread::id State::mainThreadId() {
    return m_mainThreadId;
}

bool State::crashed() {
    return m_crashed;
}

InterfaceType State::interfaceType() {
    return m_interfaceType;
}

bool State::rotateZoomedMap() {
    return m_rotateZoomedMap;
}

double State::zoomedMapScale() {
    return m_zoomedMapScale;
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

bool State::tileTextVisible() {
    return m_tileTextVisible;
}

bool State::tileFogVisible() {
    return m_tileFogVisible;
}

bool State::wireframeMode() {
    return m_wireframeMode;
}

bool State::paused() {
    return m_paused;
}

double State::simSpeed() {
    return m_simSpeed;
}

bool State::inputButtonPressed(int inputButton) {
    ASSERT(0 <= inputButton && inputButton <= 9);
    return m_inputButtons.at(inputButton);
}

void State::setCrashed() {
    m_crashed = true;
    SimUtilities::print(P()->crashMessage());
}

void State::setInterfaceType(InterfaceType interfaceType) {
    ASSERT(m_interfaceType == UNDECLARED);
    m_interfaceType = interfaceType;
}

void State::setRotateZoomedMap(bool rotateZoomedMap) {
    m_rotateZoomedMap = rotateZoomedMap;
}

void State::setZoomedMapScale(double zoomedMapScale) {
    if (zoomedMapScale < P()->minZoomedMapScale()) {
        m_zoomedMapScale = P()->minZoomedMapScale();
    }
    else if (P()->maxZoomedMapScale() < zoomedMapScale) {
        m_zoomedMapScale = P()->maxZoomedMapScale();
    }
    else {
        m_zoomedMapScale = zoomedMapScale;
    }
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

void State::setTileTextVisible(bool tileTextVisible) {
    m_tileTextVisible = tileTextVisible;
}

void State::setTileFogVisible(bool tileFogVisible) {
    m_tileFogVisible = tileFogVisible;
}

void State::setWireframeMode(bool wireframeMode) {
    m_wireframeMode = wireframeMode;
}

void State::setPaused(bool paused) {
    m_paused = paused;
}

void State::setSimSpeed(double simSpeed) {
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
