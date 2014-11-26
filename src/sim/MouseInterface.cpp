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

bool MouseInterface::wallFront(){
    return m_mouse->wallFront();
}

bool MouseInterface::wallRight(){
    return m_mouse->wallRight();
}

bool MouseInterface::wallLeft(){
    return m_mouse->wallLeft();
}

void MouseInterface::moveForward(){
    
    for (int i = 0; i < S()->SLEEP_TIME(); i += 1) {
        while (S()->PAUSED()) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->moveForward();
}

void MouseInterface::turnRight(){

    for (int i = 0; i < S()->SLEEP_TIME(); i += 1) {
        while (S()->PAUSED()) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->turnRight();
}

void MouseInterface::turnLeft(){

    for (int i = 0; i < S()->SLEEP_TIME(); i += 1) {
        while (S()->PAUSED()) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->turnLeft();
}

void MouseInterface::turnAround(){
    turnRight();
    turnRight();
}

bool MouseInterface::undoRequested(){
    return S()->UNDO_REQUESTED();
}

bool MouseInterface::resetRequested(){
    return S()->RESET_REQUESTED();
}

void MouseInterface::undoHonored(){
    S()->SET_UNDO_REQUESTED(false);
}

void MouseInterface::resetHonored(){
    S()->SET_RESET_REQUESTED(false);
}

void MouseInterface::resetPosition() {
    m_mouse->resetPosition();    
}

void MouseInterface::resetColors(int curX, int curY) {
    m_mouse->resetColors(curX, curY);
}

} // namespace sim
