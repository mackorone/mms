#include "State.h"

#include "Assert.h"
#include "Key.h"
#include "Logging.h"
#include "Param.h"

namespace mms {

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
    m_layoutType = STRING_TO_LAYOUT_TYPE.value(P()->defaultLayoutType());
    m_rotateZoomedMap = P()->defaultRotateZoomedMap();
    m_zoomedMapScale = P()->defaultZoomedMapScale();
    m_wallTruthVisible = P()->defaultWallTruthVisible();
    m_tileColorsVisible = P()->defaultTileColorsVisible();
    m_tileTextVisible = P()->defaultTileTextVisible();
    m_tileDistanceVisible = P()->defaultTileDistanceVisible();
    m_headerVisible = P()->defaultHeaderVisible();
    m_tileFogVisible = P()->defaultTileFogVisible();
    m_paused = P()->defaultPaused();
    m_simSpeed = P()->defaultSimSpeed();
    for (int i = 0; i < 10; i += 1) {
        m_inputButtons.insert(i, false);
    }
    for (Key key : ARROW_KEYS) {
        m_arrowKeys.insert(key, false);
    }
}

QString State::runId() {
    return m_runId;
}

void State::setRunId(const QString& runId) {
    ASSERT_TR(m_runId.isEmpty());
    m_runId = runId;
}

bool State::crashed() {
    return m_crashed;
}

void State::setCrashed() {
    m_crashed = true;
    qWarning().noquote().nospace() << P()->crashMessage();
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
    ASSERT_TR(m_inputButtons.contains(inputButton));
    return m_inputButtons.value(inputButton);
}

void State::setInputButtonWasPressed(int inputButton, bool pressed) {
    ASSERT_TR(m_inputButtons.contains(inputButton));
    m_inputButtons[inputButton] = pressed;
}

bool State::arrowKeyIsPressed(Key key) {
    ASSERT_TR(m_arrowKeys.contains(key));
    return m_arrowKeys.value(key);
}

void State::setArrowKeyIsPressed(Key key, bool pressed) {
    ASSERT_TR(m_arrowKeys.contains(key));
    m_arrowKeys[key] = pressed;
    qDebug() << m_arrowKeys; // TODO: MACK
}

bool State::crossesDefault(double current, double next, double defaultValue) {
    return (
        (current < defaultValue && defaultValue <    next) ||
        (   next < defaultValue && defaultValue < current)
    );
}

} // namespace mms
