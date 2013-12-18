#include "MouseInterface.h"

#include <GL/freeglut.h>
#include <unistd.h>
#include "Parameters.h"

MouseInterface::MouseInterface(Mouse* mouse, int* sleepTime, bool* paused) : 
        m_mouse(mouse), m_sleepTime(sleepTime), m_paused(paused)
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
        while (*m_paused){usleep(100*SLEEP_TIME_MIN);}; // Wait if paused
        usleep(*m_sleepTime*100);
    }
}

void MouseInterface::turnRight(){
    m_mouse->turnRight();
    // Break the sleep into 10 steps, allow pause interrupt
    for (int i = 0; i < 10; i++){
        while (*m_paused){usleep(100*SLEEP_TIME_MIN);}; // Wait if paused
        usleep(*m_sleepTime*100);
    }
}

void MouseInterface::turnLeft(){
    m_mouse->turnLeft();
    // Break the sleep into 10 steps, allow pause interrupt
    for (int i = 0; i < 10; i++){
        while (*m_paused){usleep(100*SLEEP_TIME_MIN);}; // Wait if paused
        usleep(*m_sleepTime*100);
    }
}
