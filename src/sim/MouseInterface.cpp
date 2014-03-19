#include "MouseInterface.h"

#include <GL/freeglut.h>
#include <unistd.h>
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
    m_mouse->moveForward();
    // Break the sleep into 10 steps, allow pause interrupt
    for (int i = 0; i < 10; i++){
        while (*m_paused){usleep(10*SLEEP_TIME_MIN);}; // Wait if paused
        usleep(*m_sleepTime*100);
    }
}

void MouseInterface::turnRight(){
    m_mouse->turnRight();
    // Break the sleep into 10 steps, allow pause interrupt
    for (int i = 0; i < 10; i++){
        while (*m_paused){usleep(10*SLEEP_TIME_MIN);}; // Wait if paused
        usleep(*m_sleepTime*100);
    }
}

void MouseInterface::turnLeft(){
    m_mouse->turnLeft();
    // Break the sleep into 10 steps, allow pause interrupt
    for (int i = 0; i < 10; i++){
        while (*m_paused){usleep(10*SLEEP_TIME_MIN);}; // Wait if paused
        usleep(*m_sleepTime*100);
    }
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
