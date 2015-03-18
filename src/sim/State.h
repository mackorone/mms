#pragma once

#include "InterfaceType.h"

namespace sim {

// Wrapper for the static call to State::getInstance()
class State;
State* S();

class State {

public:

    // Returns a pointer to the singeton state object
    static State* getInstance();

    // Accessors
    InterfaceType interfaceType();
    bool mazeVisible();
    bool mouseVisible();
    bool mousePathVisible(); // TODO: Implement the mouse path
    bool paused();
    bool resetRequested();
    bool undoRequested();

    // Mutators
    void setInterfaceType(InterfaceType interfaceType);
    void setMazeVisible(bool mazeVisible);
    void setMouseVisible(bool mouseVisible);
    void setMousePathVisible(bool mousePathVisible);
    void setPaused(bool paused);
    void setUndoRequested(bool undoRequested);
    void setResetRequested(bool resetRequested);

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    InterfaceType m_interfaceType;
    bool m_mazeVisible;
    bool m_mouseVisible;
    bool m_mousePathVisible;
    bool m_paused;
    bool m_resetRequested;
    bool m_undoRequested;
};

} // namespace sim
