#include "MouseInterface.h"

#include <GL/freeglut.h>
#include <unistd.h>

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
    m_mouse->moveForward();
    usleep(SLEEP_TIME*1000);
}

void MouseInterface::turnRight(){
    m_mouse->turnRight();
    usleep(SLEEP_TIME*1000);
}

void MouseInterface::turnLeft(){
    m_mouse->turnLeft();
    usleep(SLEEP_TIME*1000);
}
