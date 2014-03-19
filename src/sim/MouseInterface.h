#ifndef SIM_MOUSEINTERFACE_H_
#define SIM_MOUSEINTERFACE_H_

#include "Mouse.h"

namespace sim{

class MouseInterface{

public:
    MouseInterface(Mouse* mouse, int* sleepTime, bool* paused, bool* undoRequested, bool* resetRequested);
    ~MouseInterface();

    // ---------------------- Mouse Interfact Methods ----------------------- //

    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnRight();
    void turnLeft();

    bool undoRequested();
    bool resetRequested();
    void undoHonored();
    void resetHonored();


    // ------ Simulation utilities (not applicable to the real mouse) ------- //

    void resetPosition(); // Sets the position of the mouse to (0,0)
    void resetColors(int curX, int curY); // Resets the tile colors of the maze

private:
    Mouse* m_mouse;
    int* m_sleepTime;
    bool* m_paused;
    bool* m_undoRequested;
    bool* m_resetRequested;
};

} // namespace sim

#endif // SIM_MOUSEINTERFACE_H_
