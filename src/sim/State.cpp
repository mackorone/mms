#include "State.h"

#include "Assert.h"
#include "Keys.h"
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
    m_runId = "";
    m_mainThreadId = std::this_thread::get_id();
    m_mainLoopEntered = false;
    m_crashed = false;
    m_interfaceType = DISCRETE; // Arbitrary default value
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
        m_inputButtons.insert(std::make_pair(i, false));
    }
    for (int key : ARROW_KEYS) {
        m_arrowKeys.insert(std::make_pair(key, false));
    }
}

std::string State::runId() {
    return m_runId;
}

std::thread::id State::mainThreadId() {
    return m_mainThreadId;
}

bool State::mainLoopEntered() {
    return m_mainLoopEntered;
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

bool State::inputButtonWasPressed(int inputButton) {
    ASSERT(0 <= inputButton && inputButton <= 9);
    ASSERT(1 == m_inputButtons.count(inputButton));
    return m_inputButtons.at(inputButton);
}

bool State::arrowKeyIsPressed(int key) {
    ASSERT(1 == m_arrowKeys.count(key));
    return m_arrowKeys.at(key);
}

void State::setRunId(const std::string& runId) {
    ASSERT_EQUAL(m_runId, "");
    m_runId = runId;
}

void State::enterMainLoop() {
    ASSERT(std::this_thread::get_id() == m_mainThreadId);
    ASSERT(!m_mainLoopEntered);
    m_mainLoopEntered = true;
}

void State::setCrashed() {
    m_crashed = true;
    SimUtilities::print(P()->crashMessage());
}

void State::setInterfaceType(InterfaceType interfaceType) {
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

void State::setInputButtonWasPressed(int inputButton, bool pressed) {
    ASSERT(0 <= inputButton && inputButton <= 9);
    ASSERT(1 == m_inputButtons.count(inputButton));
    m_inputButtons.at(inputButton) = pressed;
}

void State::setArrowKeyIsPressed(int key, bool pressed) {
    ASSERT(1 == m_arrowKeys.count(key));
    m_arrowKeys.at(key) = pressed;
}

} // namespace sim
