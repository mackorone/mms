#include "State.h"

#include "Assert.h"
#include "Key.h"
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
    m_crashed = false;
    m_interfaceType; // Undefined default value
    m_layout = STRING_TO_LAYOUT.at(P()->defaultLayout());
    m_rotateZoomedMap = P()->defaultRotateZoomedMap();
    m_zoomedMapScale = P()->defaultZoomedMapScale();
    m_wallTruthVisible = P()->defaultWallTruthVisible();
    m_tileColorsVisible = P()->defaultTileColorsVisible();
    m_tileTextVisible = P()->defaultTileTextVisible();
    m_tileDistanceVisible = P()->defaultTileDistanceVisible();
    m_tileFogVisible = P()->defaultTileFogVisible();
    m_wireframeMode = P()->defaultWireframeMode();
    m_paused = P()->defaultPaused();
    m_simSpeed = P()->discreteInterfaceDefaultSpeed();
    for (int i = 0; i < 10; i += 1) {
        m_inputButtons.insert(std::make_pair(i, false));
    }
    for (Key key : ARROW_KEYS) {
        m_arrowKeys.insert(std::make_pair(key, false));
    }
}

std::string State::runId() {
    return m_runId;
}

bool State::crashed() {
    return m_crashed;
}

InterfaceType State::interfaceType() {
    return m_interfaceType;
}

Layout State::layout() {
    return m_layout;
}

bool State::rotateZoomedMap() {
    return m_rotateZoomedMap;
}

double State::zoomedMapScale() {
    return m_zoomedMapScale;
}

bool State::wallTruthVisible() {
    return m_wallTruthVisible;
}

bool State::tileColorsVisible() {
    return m_tileColorsVisible;
}

bool State::tileFogVisible() {
    return m_tileFogVisible;
}

bool State::tileTextVisible() {
    return m_tileTextVisible;
}

bool State::tileDistanceVisible() {
    return m_tileDistanceVisible;
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
    ASSERT(SimUtilities::mapContains(m_inputButtons, inputButton));
    return m_inputButtons.at(inputButton);
}

bool State::arrowKeyIsPressed(Key key) {
    ASSERT(SimUtilities::mapContains(m_arrowKeys, key));
    return m_arrowKeys.at(key);
}

void State::setRunId(const std::string& runId) {
    ASSERT_EQUAL(m_runId, "");
    m_runId = runId;
}

void State::setCrashed() {
    m_crashed = true;
    SimUtilities::print(P()->crashMessage());
}

void State::setInterfaceType(InterfaceType interfaceType) {
    m_interfaceType = interfaceType;
}

void State::setLayout(Layout layout) {
    m_layout = layout;
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

void State::setWallTruthVisible(bool wallTruthVisible) {
    m_wallTruthVisible = wallTruthVisible;
}

void State::setTileColorsVisible(bool tileColorsVisible) {
    m_tileColorsVisible = tileColorsVisible;
}

void State::setTileFogVisible(bool tileFogVisible) {
    m_tileFogVisible = tileFogVisible;
}

void State::setTileTextVisible(bool tileTextVisible) {
    m_tileTextVisible = tileTextVisible;
}

void State::setTileDistanceVisible(bool tileDistanceVisible) {
    m_tileDistanceVisible = tileDistanceVisible;
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
    ASSERT(SimUtilities::mapContains(m_inputButtons, inputButton));
    m_inputButtons.at(inputButton) = pressed;
}

void State::setArrowKeyIsPressed(Key key, bool pressed) {
    ASSERT(SimUtilities::mapContains(m_arrowKeys, key));
    m_arrowKeys.at(key) = pressed;
}

} // namespace sim
