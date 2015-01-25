#include "MouseInterface.h"

#include <GL/freeglut.h>

#include "Param.h"
#include "State.h"
#include "Utilities.h"

namespace sim {

MouseInterface::MouseInterface(Mouse* mouse) : m_mouse(mouse)
{ }

MouseInterface::~MouseInterface()
{ }

bool MouseInterface::wallFront() {
    return m_mouse->wallFront();
}

bool MouseInterface::wallRight() {
    return m_mouse->wallRight();
}

bool MouseInterface::wallLeft() {
    return m_mouse->wallLeft();
}

void MouseInterface::moveForward() {

    // Implement the sleeping here
    /*
    float distance = 1.0; // meters
    float speed = 10.0; // meters per second
    float duration = distance/speed;

    int numSteps = 100; // TODO this is hard coded now
    
    
    for (int i = 0; i < numSteps; i += 1) {
        while (S()->paused()) {sim::sleep(std::Milliseconds(1));} // Sleep while paused
        m_mouse->moveForward(distance/numSteps);
        sim::sleep(duration/numSteps*1000);
    }
    */

    /*
    // Move the mouse forward
    m_mouse->moveForward(0.16, 1.0);
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
    m_mouse->resetPosition();    
}

void MouseInterface::resetColors(int curX, int curY) {
    m_mouse->resetColors(curX, curY);
}

} // namespace sim
