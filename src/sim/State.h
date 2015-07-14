#pragma once

#include <map>
#include <thread>

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
    std::thread::id mainThreadId();
    bool mainLoopEntered();
    bool crashed();
    InterfaceType interfaceType();
    bool rotateZoomedMap();
    double zoomedMapScale();
    bool mousePathVisible(); // TODO: Implement the mouse path
    bool wallTruthVisible();
    bool tileColorsVisible();
    bool tileTextVisible();
    bool tileFogVisible();
    bool wireframeMode();
    bool paused();
    double simSpeed();
    bool inputButtonWasPressed(int inputButton);
    bool arrowKeyIsPressed(int key);

    // Mutators
    void enterMainLoop();
    void setCrashed();
    void setInterfaceType(InterfaceType interfaceType);
    void setRotateZoomedMap(bool rotateZoomedMap);
    void setZoomedMapScale(double zoomedMapScale);
    void setMousePathVisible(bool mousePathVisible);
    void setWallTruthVisible(bool wallTruthVisible);
    void setTileColorsVisible(bool tileColorsVisible);
    void setTileTextVisible(bool tileTextVisible);
    void setTileFogVisible(bool tileFogVisible);
    void setWireframeMode(bool wireframeMode);
    void setPaused(bool paused);
    void setSimSpeed(double simSpeed);
    void setInputButtonWasPressed(int inputButton, bool pressed);
    void setArrowKeyIsPressed(int key, bool pressed);

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    std::thread::id m_mainThreadId;
    bool m_mainLoopEntered;
    bool m_crashed;
    InterfaceType m_interfaceType;
    bool m_rotateZoomedMap;
    double m_zoomedMapScale;
    bool m_mousePathVisible;
    bool m_wallTruthVisible;
    bool m_tileColorsVisible;
    bool m_tileTextVisible;
    bool m_tileFogVisible;
    bool m_wireframeMode;
    bool m_paused;
    double m_simSpeed;
    std::map<int, bool> m_inputButtons;
    std::map<int, bool> m_arrowKeys;
};

} // namespace sim
