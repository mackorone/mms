#include "Forward.h"

void Forward::solve(sim::MouseInterface* mouse) {
    mouse->declareInterfaceType(sim::DISCRETE);
    while (true) {
        mouse->moveForward();
    }
}
