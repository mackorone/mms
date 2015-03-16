#pragma once

namespace sim {

// TODO: This uses the singleton pattern

// Wrapper for the static call to State::getInstance()
class State;
State* S();

class State {

public:

    // Returns a pointer to the singeton state object
    static State* getInstance();

    int  simSpeed();
    bool paused();          // Initially set to false
    bool undoRequested();  // Whether or not an undo was requested
    bool resetRequested(); // Whether or not a reset was requested

    void setSimSpeed(int simSpeed);
    void setPaused(bool paused);
    void setUndoRequested(bool undoRequested); // TODO: Fix these params
    void setResetRequested(bool resetRequested); // TODO: Fix these params

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    int m_simSpeed; // Percentage of real-time
    bool m_paused; // Initially set to false
    bool m_undoRequested; // Whether or not an undo was requested
    bool m_resetRequested; // Whether or not a reset was requested
};

} // namespace sim
