#include "MouseInterface.h"

#include <GL/freeglut.h>

#include <Meters.h>
#include <MetersPerSecond.h>
#include <Milliseconds.h>
#include <Seconds.h>

#include "Param.h"
#include "State.h"
#include "SimUtilities.h"

namespace sim {

MouseInterface::MouseInterface(Mouse* mouse) : m_mouse(mouse)
{ }

MouseInterface::~MouseInterface()
{ }

void MouseInterface::setWheelSpeeds(float leftWheelSpeed, float rightWheelSpeed) {
    m_mouse->setWheelSpeeds(RadiansPerSecond(leftWheelSpeed), RadiansPerSecond(rightWheelSpeed));
}

float MouseInterface::read(std::string name) {
    // TODO: Delay sensor-read-time here
    return m_mouse->read(name);
}

bool MouseInterface::wallFront() {
    //return m_mouse->wallFront();
    return false;
}

bool MouseInterface::wallRight() {
    //return m_mouse->wallRight();
    return false;
}

bool MouseInterface::wallLeft() {
    //return m_mouse->wallLeft();
    return false;
}

void MouseInterface::moveForward() {

    // TODO: Acceleration

    // Implement the sleeping here
    /*
    Meters distance(P()->wallLength() + P()->wallWidth());
    MetersPerSecond speed(1.0);
    Seconds duration(distance/speed);

    int numSteps = 10; // TODO this is hard coded now
    
    for (int i = 0; i < numSteps; i += 1) {
        while (S()->paused()) {
            sim::sleep(Milliseconds(1)); // Sleep while paused
        }
        m_mouse->moveForward(distance.getMeters()/numSteps);
        sim::sleep(Seconds(duration.getSeconds()/numSteps));
    }
    */
}

void MouseInterface::turnRight() {

    /*
    for (int i = 0; i < S()->simSpeed(); i += 1) {
        while (S()->paused()) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->turnRight();
    */
}

void MouseInterface::turnLeft() {

    /*
    for (int i = 0; i < S()->simSpeed(); i += 1) {
        while (S()->paused()) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->turnLeft();
    */
}

void MouseInterface::turnAround() {
    turnRight();
    turnRight();
}

bool MouseInterface::undoRequested() {
    return S()->undoRequested();
}

bool MouseInterface::resetRequested() {
    return S()->resetRequested();
}

void MouseInterface::undoHonored() {
    S()->setUndoRequested(false);
}

void MouseInterface::resetHonored() {
    S()->setResetRequested(false);
}

void MouseInterface::resetPosition() {
    //m_mouse->resetPosition();    
}

void MouseInterface::resetColors(int curX, int curY) {
    //m_mouse->resetColors(curX, curY);
}

} // namespace sim
