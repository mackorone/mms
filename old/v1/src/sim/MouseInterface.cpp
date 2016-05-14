#include "MouseInterface.h"

#include <GL/freeglut.h>

#include "Parameters.h"
#include "Sleep.h"

namespace sim{

MouseInterface::MouseInterface(Mouse* mouse, int* sleepTime, bool* paused, bool* undoRequested,
                                                                           bool* resetRequested) : 
        m_mouse(mouse), m_sleepTime(sleepTime), m_paused(paused),
        m_undoRequested(undoRequested), m_resetRequested(resetRequested)
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
    
    for (int i = 0; i < *m_sleepTime; i += 1) {
        while (*m_paused) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->moveForward();
}

void MouseInterface::turnRight(){

    for (int i = 0; i < *m_sleepTime; i += 1) {
        while (*m_paused) {sim::sleep(1);} // Sleep while paused
        sim::sleep(1); // Sleep for 1 ms of the total m_sleepTime
    }

    // Move the mouse forward
    m_mouse->turnRight();
}

void MouseInterface::turnLeft(){

    for (int i = 0; i < *m_sleepTime; i += 1) {
        while (*m_paused) {sim::sleep(1);} // Sleep while paused
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
    return *m_undoRequested;
}

bool MouseInterface::resetRequested(){
    return *m_resetRequested;
}

void MouseInterface::undoHonored(){
    *m_undoRequested = false;
}

void MouseInterface::resetHonored(){
    *m_resetRequested = false;
}

void MouseInterface::resetPosition() {
    m_mouse->resetPosition();    
}

void MouseInterface::resetColors(int curX, int curY) {
    m_mouse->resetColors(curX, curY);
}

} // namespace sim
