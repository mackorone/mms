#ifndef MOUSEINTERFACE_H_
#define MOUSEINTERFACE_H_

#include "Mouse.h"

class MouseInterface{

public:
    MouseInterface(Mouse* mouse, int* sleepTime, bool* paused);
    ~MouseInterface();

    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnRight();
    void turnLeft();

private:
    Mouse* m_mouse;
    int* m_sleepTime;
    bool* m_paused;
};
#endif // MOUSEINTERFACE_H_
