#pragma once

#include <map>
#include <thread>

#include "InterfaceType.h"
#include "Key.h"
#include "Layout.h"

namespace sim {

// Wrapper for the static call to State::getInstance()
class State;
State* S();

class State {

public:

    // Returns a pointer to the singleton state object
    static State* getInstance();

    // Accessors
    std::string runId();
    bool crashed();
    InterfaceType interfaceType();
    Layout layout();
    bool rotateZoomedMap();
    double zoomedMapScale();
    bool wallTruthVisible();
    bool tileColorsVisible();
    bool tileTextVisible();
    bool tileFogVisible();
    bool wireframeMode();
    bool paused();
    double simSpeed();
    bool inputButtonWasPressed(int inputButton);
    bool arrowKeyIsPressed(Key key);

    // Mutators
    void setRunId(const std::string& runId);
    void setCrashed();
    void setInterfaceType(InterfaceType interfaceType);
    void setLayout(Layout layout);
    void setRotateZoomedMap(bool rotateZoomedMap);
    void setZoomedMapScale(double zoomedMapScale);
    void setWallTruthVisible(bool wallTruthVisible);
    void setTileColorsVisible(bool tileColorsVisible);
    void setTileTextVisible(bool tileTextVisible);
    void setTileFogVisible(bool tileFogVisible);
    void setWireframeMode(bool wireframeMode);
    void setPaused(bool paused);
    void setSimSpeed(double simSpeed);
    void setInputButtonWasPressed(int inputButton, bool pressed);
    void setArrowKeyIsPressed(Key key, bool pressed);

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    std::string m_runId;
    bool m_crashed;
    InterfaceType m_interfaceType;
    Layout m_layout;
    bool m_rotateZoomedMap;
    double m_zoomedMapScale;
    bool m_wallTruthVisible;
    bool m_tileColorsVisible;
    bool m_tileTextVisible;
    bool m_tileFogVisible;
    bool m_wireframeMode;
    bool m_paused;
    double m_simSpeed;
    std::map<int, bool> m_inputButtons;
    std::map<Key, bool> m_arrowKeys;
};

} // namespace sim
