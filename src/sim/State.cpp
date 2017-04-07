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
