#include "Continue.h"

void Continue::solve(sim::MouseInterface* mouse) {

    //while (true) {
        mouse->moveForward();
        mouse->turnRight();
        mouse->moveForward();
        mouse->turnLeft();
        mouse->moveForward();
    //}
    // ---- REMINDER ---- //
    /* Valid function calls:
     * 1) mouse->wallFront()
     * 2) mouse->wallRight()
     * 3) mouse->wallLeft()
     * 4) mouse->moveForward()
     * 5) mouse->turnRight()
     * 6) mouse->turnLeft()
     */
}
