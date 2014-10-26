#include "MouseInterface.h"

#include <GL/freeglut.h>
//#include <unistd.h>
#include "Methods.h"
#include "Parameters.h"

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

    // Break the sleep into 2 parts of 5 steps, allow pause interrupt

    for (int i = 0; i < 5; i++){
        while (*m_paused){sim::sleep(SLEEP_TIME_MIN);}; // Wait if paused
        sim::sleep(*m_sleepTime);
    }

    m_mouse->moveForward();

    for (int i = 0; i < 5; i++){
        while (*m_paused){sim::sleep(SLEEP_TIME_MIN);}; // Wait if paused
        sim::sleep(*m_sleepTime);
    }
}

void MouseInterface::turnRight(){

    // Break the sleep into 2 parts of 5 steps, allow pause interrupt

    for (int i = 0; i < 5; i++){
        while (*m_paused){sim::sleep(SLEEP_TIME_MIN);}; // Wait if paused
        sim::sleep(*m_sleepTime);
    }

    m_mouse->turnRight();

    for (int i = 0; i < 5; i++){
        while (*m_paused){sim::sleep(SLEEP_TIME_MIN);}; // Wait if paused
        sim::sleep(*m_sleepTime);
    }
}

void MouseInterface::turnLeft(){

    // Break the sleep into 2 parts of 5 steps, allow pause interrupt

    for (int i = 0; i < 5; i++){
        while (*m_paused){sim::sleep(SLEEP_TIME_MIN);}; // Wait if paused
        sim:sleep(*m_sleepTime);
    }

    m_mouse->turnLeft();

    for (int i = 0; i < 5; i++){
        while (*m_paused){sim::sleep(SLEEP_TIME_MIN);}; // Wait if paused
        sim::sleep(*m_sleepTime);
    }
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
    // Put a delay in here so that we have to actually reset the robot
    m_mouse->resetPosition();    
}

void MouseInterface::resetColors(int curX, int curY) {
    m_mouse->resetColors(curX, curY);
}

} // namespace sim
