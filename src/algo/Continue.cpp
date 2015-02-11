#include "Continue.h"

#include "../sim/Utilities.h"
#include "../sim/units/Seconds.h"

void Continue::solve(sim::MouseInterface* mouse) {

    //mouse->getRightWheel()->setAngularVelocity(RadiansPerSecond(1.0));
    //mouse->getLeftWheel()->setAngularVelocity(RadiansPerSecond(1.0));
    while (true) {
        sim::sleep(sim::Seconds(0.1));
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
