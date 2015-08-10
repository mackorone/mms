#include "Continuous.h"

#include <cmath>
#include <iostream>
namespace continuous {

std::string Continuous::mouseFile() const {
    return "megaMouse.xml";
}

sim::InterfaceType Continuous::interfaceType() const {
    return sim::CONTINUOUS;
}

void Continuous::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {

    m_mouse = mouse;

    while (true) {
        if (!wallRight()) {
            m_mouse->delay(350);
            turnRight();
        }
        while (wallFront()) {
            turnLeft();
        }
        moveForward();
        correctErrors();
    }
}

bool Continuous::wallRight() {
    return m_mouse->read("right") > 0.5;
}

bool Continuous::wallLeft() {
    return m_mouse->read("left") > 0.5;
}

bool Continuous::wallFront() {
    return m_mouse->read("rightFront") > 0.9;
}

void Continuous::turnRight() {
    m_mouse->setWheelSpeeds(-5*M_PI, -5*M_PI);
    m_mouse->delay(290);
    m_mouse->setWheelSpeeds(-10*M_PI, 10*M_PI);
    m_mouse->delay(100);
    m_mouse->setWheelSpeeds(-0, 0);
    
}

void Continuous::turnLeft() {
    m_mouse->setWheelSpeeds(5*M_PI, 5*M_PI);
    m_mouse->delay(290);
    m_mouse->setWheelSpeeds(-10*M_PI, 10*M_PI);
    m_mouse->delay(100);
    m_mouse->setWheelSpeeds(-0, 0);
}

void Continuous::moveForward() {
    //m_mouse->setWheelSpeeds(-10*M_PI, 10*M_PI);
    //m_mouse->delay(280);
int Kp;
    double error;
    double totalError;
    if (wallRight() && wallLeft()) {
        error = m_mouse->read("left") - m_mouse->read("right");
        Kp = 20;
    }

    else if (wallRight()) {
        error = .5 * (0.776 - m_mouse->read("right"));
	Kp = 5;
    }
    
    else if (wallLeft()) {
        error = .5 * (m_mouse->read("left") - 0.776);
        Kp = 5;
    }

    else {
        error = 0;
    }

    totalError = Kp * error;
    m_mouse->setWheelSpeeds(-(10*M_PI + totalError), 10*M_PI - totalError);
    m_mouse->delay(1);
    //std::cout << m_mouse->read("left") << std::endl;
    //std::cout << m_mouse->read("leftMiddle") << std::endl;
    //std::cout << std::endl;
    //m_mouse->setWheelSpeeds(0, 0);
    //m_mouse->delay(0);
}

void Continuous::correctErrors() {
    
    //m_mouse->delay(10);
/*
    if (m_mouse->read("rightMiddle") < 0.65) {
        // Right
        m_mouse->setWheelSpeeds(-5, -5);
        m_mouse->delay(10);
    }
    if (m_mouse->read("rightMiddle") > 0.8) {
        // Left
        m_mouse->setWheelSpeeds(5, 5);
        m_mouse->delay(10);
    }
*/
}

} // namespace continuous
