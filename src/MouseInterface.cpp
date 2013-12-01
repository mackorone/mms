#include "MouseInterface.h"

#include <GL/freeglut.h>
#include <unistd.h>
#include "Parameters.h"

// TODO: Put while(pause) in all of the methods here to allow pausing

MouseInterface::MouseInterface(Mouse* mouse, int* sleepTime, bool* paused) : 
        m_mouse(mouse), m_sleepTime(sleepTime), m_paused(paused)
{ }

MouseInterface::~MouseInterface()
{ }

bool MouseInterface::wallFront(){
    while (*m_paused){}; // Wait if paused
    return m_mouse->wallFront();
}

bool MouseInterface::wallRight(){
    while (*m_paused){}; // Wait if paused
    return m_mouse->wallRight();
}

bool MouseInterface::wallLeft(){
    while (*m_paused){}; // Wait if paused
    return m_mouse->wallLeft();
}

void MouseInterface::moveForward(){
    while (*m_paused){}; // Wait if paused
    m_mouse->moveForward();
    for (int i = 0; i < 1000; i++){
        while (*m_paused){}; // Wait if paused
        usleep(*m_sleepTime);
    }
}

void MouseInterface::turnRight(){
    while (*m_paused){}; // Wait if paused
    m_mouse->turnRight();
    for (int i = 0; i < 1000; i++){
        while (*m_paused){}; // Wait if paused
        usleep(*m_sleepTime);
    }
}

void MouseInterface::turnLeft(){
    while (*m_paused){}; // Wait if paused
    m_mouse->turnLeft();
    for (int i = 0; i < 1000; i++){
        while (*m_paused){}; // Wait if paused
        usleep(*m_sleepTime);
    }
}
