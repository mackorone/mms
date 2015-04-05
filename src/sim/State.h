#pragma once

#include <map>

#include "InterfaceType.h"

namespace sim {

// Wrapper for the static call to State::getInstance()
class State;
State* S();

class State {

public:

    // Returns a pointer to the singleton state object
    static State* getInstance();

    // Accessors
    bool crashed();
    InterfaceType interfaceType();
    bool mazeVisible();
    bool mouseVisible();
    bool mousePathVisible(); // TODO: Implement the mouse path
    bool wallTruthVisible();
    bool tileColorsVisible();
    bool paused();
    float simSpeed();
    bool inputButtonPressed(int inputButton);

    // Mutators
    void setCrashed();
    void setInterfaceType(InterfaceType interfaceType);
    void setMazeVisible(bool mazeVisible);
    void setMouseVisible(bool mouseVisible);
    void setMousePathVisible(bool mousePathVisible);
    void setWallTruthVisible(bool wallTruthVisible);
    void setTileColorsVisible(bool tileColorsVisible);
    void setPaused(bool paused);
    void setSimSpeed(float simSpeed);
    void setInputButtonPressed(int inputButton, bool pressed);

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    bool m_crashed;
    InterfaceType m_interfaceType;
    bool m_mazeVisible;
    bool m_mouseVisible;
    bool m_mousePathVisible;
    bool m_wallTruthVisible;
    bool m_tileColorsVisible;
    bool m_paused;
    float m_simSpeed;
    std::map<int, bool> m_inputButtons;
};

} // namespace sim
