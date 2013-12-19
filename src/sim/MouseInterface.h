#ifndef SIM_MOUSEINTERFACE_H_
#define SIM_MOUSEINTERFACE_H_

#include "Mouse.h"

namespace sim{

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

} // namespace sim

#endif // SIM_MOUSEINTERFACE_H_
