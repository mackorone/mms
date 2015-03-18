#include "Continue.h"

#include <cmath>

void Continue::solve(sim::MouseInterface* mouse) {

    // Initialize the member variable
    m_mouse = mouse;

    while (true) {
        if (!wallRight()) {
            turnRight();
        }
        while (wallFront()) {
            turnLeft();
        }
        moveForward();
        correctErrors();
    }
}

bool Continue::wallRight() {
    return m_mouse->read("C") > 0.5;
}

bool Continue::wallFront() {
    return m_mouse->read("A") > 0.75;
}

void Continue::turnRight() {
    m_mouse->setWheelSpeeds(-5*M_PI, -5*M_PI);
    m_mouse->delay(132);
    m_mouse->setWheelSpeeds(0, 0);
}

void Continue::turnLeft() {
    m_mouse->setWheelSpeeds(5*M_PI, 5*M_PI);
    m_mouse->delay(132);
    m_mouse->setWheelSpeeds(0, 0);
}

void Continue::moveForward() {
    m_mouse->setWheelSpeeds(-10*M_PI, 10*M_PI);
    m_mouse->delay(150);
    for (int i = 0; i < 5; i += 1) {
        if (m_mouse->read("A") > 0.9) {
            break;
        }
        m_mouse->delay(10);
    }
    m_mouse->setWheelSpeeds(0, 0);
    m_mouse->delay(0);
}

void Continue::correctErrors() {
    if (m_mouse->read("B") < 0.3) {
        // Right
        m_mouse->setWheelSpeeds(-5, 0);
        m_mouse->delay(10);
    }
    if (m_mouse->read("B") > 0.5) {
        // Left
        m_mouse->setWheelSpeeds(0, 5);
        m_mouse->delay(10);
    }
}
