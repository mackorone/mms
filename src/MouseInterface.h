#ifndef MOUSEINTERFACE_H_
#define MOUSEINTERFACE_H_

#include "Mouse.h"

class MouseInterface{

public:
    MouseInterface(Mouse* mouse);
    ~MouseInterface();

    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnRight();
    void turnLeft();

private:
    Mouse* m_mouse;
};
#endif // MOUSEINTERFACE_H_
