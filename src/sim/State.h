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

    LayoutType layoutType();
    void setLayoutType(LayoutType layoutType);

    bool rotateZoomedMap();
    void setRotateZoomedMap(bool rotateZoomedMap);

    double zoomedMapScale();
    void setZoomedMapScale(double zoomedMapScale);

    bool wallTruthVisible();
    void setWallTruthVisible(bool wallTruthVisible);

    bool tileColorsVisible();
    void setTileColorsVisible(bool tileColorsVisible);

    bool tileFogVisible();
    void setTileFogVisible(bool tileFogVisible);

    bool tileTextVisible();
    void setTileTextVisible(bool tileTextVisible);

    bool tileDistanceVisible();
    void setTileDistanceVisible(bool tileDistanceVisible);

    bool paused();
    void setPaused(bool paused);

    double simSpeed();
    void setSimSpeed(double simSpeed);

    bool inputButtonWasPressed(int inputButton);
    void setInputButtonWasPressed(int inputButton, bool pressed);

    bool arrowKeyIsPressed(int key);
    void setArrowKeyIsPressed(int key, bool pressed);

private:

    // A private constructor is used to ensure only one instance of this class exists
    State();

    // A pointer to the actual instance of the class
    static State* INSTANCE;

    bool m_crashed;
    LayoutType m_layoutType;
    bool m_rotateZoomedMap;
    double m_zoomedMapScale;
    bool m_wallTruthVisible;
    bool m_tileColorsVisible;
    bool m_tileFogVisible;
    bool m_tileTextVisible;
    bool m_tileDistanceVisible;
    bool m_paused;
    double m_simSpeed;
    QMap<int, bool> m_inputButtons;
    QMap<int, bool> m_arrowKeys;

    // Returns true if defaultValue lies between current and next
    bool crossesDefault(double current, double next, double defaultValue);
};

} // namespace mms
