#include "Forward.h"

namespace misc {

void Forward::solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) {
    mouse->initializeMouse("mouse.xml");
    mouse->declareInterfaceType(sim::DISCRETE);
    while (true) {
        mouse->moveForward();
    }
}

} // namespace misc
