#include "State.h"

#include "Assert.h"
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
    m_crashed = false;
    m_paused = P()->defaultPaused();
    m_simSpeed = P()->defaultSimSpeed();
    for (int i = 0; i < 10; i += 1) {
        m_inputButtons.insert(i, false);
    }
    m_arrowKeys.insert(Qt::Key_Up, false);
    m_arrowKeys.insert(Qt::Key_Down, false);
    m_arrowKeys.insert(Qt::Key_Left, false);
    m_arrowKeys.insert(Qt::Key_Right, false);
}

bool State::crashed() {
    return m_crashed;
}

void State::setCrashed() {
    m_crashed = true;
    qWarning().noquote().nospace() << P()->crashMessage();
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
    if (simSpeed < 0.0) {
        m_simSpeed = 0.0;
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

bool State::arrowKeyIsPressed(int key) {
    ASSERT_TR(m_arrowKeys.contains(key));
    return m_arrowKeys.value(key);
}

void State::setArrowKeyIsPressed(int key, bool pressed) {
    ASSERT_TR(m_arrowKeys.contains(key));
    m_arrowKeys[key] = pressed;
}

} // namespace mms
