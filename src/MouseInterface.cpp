#include "MouseInterface.h"

#include <GL/freeglut.h>
#include <unistd.h>
#include "Parameters.h"

MouseInterface::MouseInterface(Mouse* mouse, int sleepTime) : m_mouse(mouse), m_sleepTime(sleepTime)
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
    usleep(m_sleepTime*1000);
}

void MouseInterface::turnRight(){
    m_mouse->turnRight();
    usleep(m_sleepTime*1000);
}

void MouseInterface::turnLeft(){
    m_mouse->turnLeft();
    usleep(m_sleepTime*1000);
}
