#include "State.h"

#include "Assert.h"
#include "ContainerUtilities.h"
#include "Key.h"
#include "Logging.h"
#include "Param.h"

namespace sim {

State* S() {
    return State::getInstance();
}

// Definition of the variable for linking
State* State::INSTANCE = nullptr;
State* State::getInstance() {
    if (nullptr == INSTANCE) {
        INSTANCE = new State();
    }
    return INSTANCE;
}

State::State() {
    m_runId = "";
    m_crashed = false;
    m_layoutType = STRING_TO_LAYOUT_TYPE.at(P()->defaultLayoutType());
    m_rotateZoomedMap = P()->defaultRotateZoomedMap();
    m_zoomedMapScale = P()->defaultZoomedMapScale();
    m_wallTruthVisible = P()->defaultWallTruthVisible();
    m_tileColorsVisible = P()->defaultTileColorsVisible();
    m_tileTextVisible = P()->defaultTileTextVisible();
    m_tileDistanceVisible = P()->defaultTileDistanceVisible();
    m_headerVisible = P()->defaultHeaderVisible();
    m_tileFogVisible = P()->defaultTileFogVisible();
    m_wireframeMode = P()->defaultWireframeMode();
    m_paused = P()->defaultPaused();
    m_simSpeed = P()->defaultSimSpeed();
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

void State::setRunId(const std::string& runId) {
    SIM_ASSERT_EQ(m_runId, "");
    m_runId = runId;
}

bool State::crashed() {
    return m_crashed;
}

void State::setCrashed() {
    m_crashed = true;
    L()->warn("%v", P()->crashMessage());
}

LayoutType State::layoutType() {
    return m_layoutType;
}

void State::setLayoutType(LayoutType layoutType) {
    m_layoutType = layoutType;
}

bool State::rotateZoomedMap() {
    return m_rotateZoomedMap;
}

void State::setRotateZoomedMap(bool rotateZoomedMap) {
    m_rotateZoomedMap = rotateZoomedMap;
}

double State::zoomedMapScale() {
    return m_zoomedMapScale;
}

void State::setZoomedMapScale(double zoomedMapScale) {
    if (zoomedMapScale < P()->minZoomedMapScale()) {
        m_zoomedMapScale = P()->minZoomedMapScale();
    }
    else if (P()->maxZoomedMapScale() < zoomedMapScale) {
        m_zoomedMapScale = P()->maxZoomedMapScale();
    }
    else {
        // Anchor to the default whenever we pass by it
        if (crossesDefault(m_zoomedMapScale, zoomedMapScale, P()->defaultZoomedMapScale())) {
            m_zoomedMapScale = P()->defaultZoomedMapScale();
        }
        else {
            m_zoomedMapScale = zoomedMapScale;
        }
    }
}

bool State::wallTruthVisible() {
    return m_wallTruthVisible;
}

void State::setWallTruthVisible(bool wallTruthVisible) {
    m_wallTruthVisible = wallTruthVisible;
}

bool State::tileColorsVisible() {
    return m_tileColorsVisible;
}

void State::setTileColorsVisible(bool tileColorsVisible) {
    m_tileColorsVisible = tileColorsVisible;
}

bool State::tileFogVisible() {
    return m_tileFogVisible;
}

void State::setTileFogVisible(bool tileFogVisible) {
    m_tileFogVisible = tileFogVisible;
}

bool State::tileTextVisible() {
    return m_tileTextVisible;
}

void State::setTileTextVisible(bool tileTextVisible) {
    m_tileTextVisible = tileTextVisible;
}

bool State::tileDistanceVisible() {
    return m_tileDistanceVisible;
}

void State::setTileDistanceVisible(bool tileDistanceVisible) {
    m_tileDistanceVisible = tileDistanceVisible;
}

bool State::headerVisible() {
    return m_headerVisible;
}

void State::setHeaderVisible(bool headerVisible) {
    m_headerVisible = headerVisible;
}

bool State::wireframeMode() {
    return m_wireframeMode;
}

void State::setWireframeMode(bool wireframeMode) {
    m_wireframeMode = wireframeMode;
}

bool State::paused() {
    return m_paused;
}

void State::setPaused(bool paused) {
    m_paused = paused;
}

double State::simSpeed() {
    return m_simSpeed;
}

void State::setSimSpeed(double simSpeed) {
    if (simSpeed < P()->minSimSpeed()) {
        m_simSpeed = P()->minSimSpeed();
    }
    else if (P()->maxSimSpeed() < simSpeed) {
        m_simSpeed = P()->maxSimSpeed();
    }
    else {
        // Anchor to the default whenever we pass by it
        if (crossesDefault(m_simSpeed, simSpeed, P()->defaultSimSpeed())) {
            m_simSpeed = P()->defaultSimSpeed();
        }
        else {
            m_simSpeed = simSpeed;
        }
    }
}

bool State::inputButtonWasPressed(int inputButton) {
    SIM_ASSERT_TR(ContainerUtilities::mapContains(m_inputButtons, inputButton));
    return m_inputButtons.at(inputButton);
}

void State::setInputButtonWasPressed(int inputButton, bool pressed) {
    SIM_ASSERT_TR(ContainerUtilities::mapContains(m_inputButtons, inputButton));
    m_inputButtons.at(inputButton) = pressed;
}

bool State::arrowKeyIsPressed(Key key) {
    SIM_ASSERT_TR(ContainerUtilities::mapContains(m_arrowKeys, key));
    return m_arrowKeys.at(key);
}

void State::setArrowKeyIsPressed(Key key, bool pressed) {
    SIM_ASSERT_TR(ContainerUtilities::mapContains(m_arrowKeys, key));
    m_arrowKeys.at(key) = pressed;
}

bool State::crossesDefault(double current, double next, double defaultValue) {
    return (
        (current < defaultValue && defaultValue <    next) ||
        (   next < defaultValue && defaultValue < current)
    );
}

} // namespace sim
