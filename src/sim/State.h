#pragma once

namespace sim {

// TODO: This uses the singleton pattern

// Wrapper for the static call to State::getInstance()
class State;
State* S();

// A parameters class
class State {

public:

    // Returns a pointer to the singeton state object
    static State* getInstance();

    int  SLEEP_TIME();      // ms between simulation steps
    bool PAUSED();          // Initially set to false  
    bool UNDO_REQUESTED();  // Whether or not an undo was requested
    bool RESET_REQUESTED(); // Whether or not a reset was requested

    void SET_SLEEP_TIME(int sleep_time);
    void SET_PAUSED(bool paused);
    void SET_UNDO_REQUESTED(bool paused); // TODO: Fix these params
    void SET_RESET_REQUESTED(bool paused); // TODO: Fix these params

private:

    // TODO: Document the singleton pattern
    State();
    // A pointer to the actual instance of the parameter object
    static State* INSTANCE;

    bool m_PAUSED; // Initially set to false  
    int m_SLEEP_TIME; // ms between simulation steps
    bool m_UNDO_REQUESTED; // Whether or not an undo was requested
    bool m_RESET_REQUESTED; // Whether or not a reset was requested
};

} // namespace sim
