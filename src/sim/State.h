#pragma once

#include <QMap>

#include "LayoutType.h"

namespace mms {

// Wrapper for the static call to State::getInstance()
class State;
State* S();

class State {

public:

    // Returns a pointer to the singleton state object
    static State* getInstance();

    // TODO: MACK - should be a property of the mouse?
    bool crashed();
    void setCrashed();

    bool paused();
    void setPaused(bool paused);

    bool inputButtonWasPressed(int inputButton);
    void setInputButtonWasPressed(int inputButton, bool pressed);

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    bool m_crashed;
    bool m_paused;
    QMap<int, bool> m_inputButtons;
};

} // namespace mms
