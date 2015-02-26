#include "Continue.h"

#include <cmath> // TODO

#include "../sim/SimUtilities.h" // TODO: Arduino utils
#include <Milliseconds.h>

void Continue::solve(sim::MouseInterface* mouse) {

    while (true) {
        mouse->setRightWheelSpeed(10*M_PI);
        mouse->setLeftWheelSpeed(-10.5*M_PI);
        sim::sleep(sim::Milliseconds(1000));
    }
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
