#include "LeftWallFollow.h"

namespace leftWallFollow {

bool LeftWallFollow::automaticallyClearFog() const {
    // Press 0 to turn off automaticallyClearFog
    // Press 1 to turn it back on
    if (m_mouse->inputButtonPressed(0)) {
        if (m_mouse->inputButtonPressed(1)) {
            m_mouse->acknowledgeInputButtonPressed(0);
            m_mouse->acknowledgeInputButtonPressed(1);
        }
        return false;
    }
    // Pressing 1 before 0 does nothing
    else if (m_mouse->inputButtonPressed(1)) {
        m_mouse->acknowledgeInputButtonPressed(1);
    }
    return true;
}

bool LeftWallFollow::declareWallOnRead() const {
    return true;
}

bool LeftWallFollow::declareBothWallHalves() const {
    return true;
}

void LeftWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    m_mouse = mouse;
    while (true) {
        leftWallFollowStep();
    }
}

void LeftWallFollow::leftWallFollowStep() {
    if (!m_mouse->wallLeft()){
        m_mouse->turnLeft();
    }
    while (m_mouse->wallFront()){
        m_mouse->turnRight();
    }
    m_mouse->moveForward();
}

} // namespace leftWallFollow
