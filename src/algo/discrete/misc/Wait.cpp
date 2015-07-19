#include "Wait.h"

namespace misc {

void Wait::solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) {
    mouse->initializeMouse("mouse.xml");
    mouse->declareInterfaceType(sim::DISCRETE);
}

} // namespace misc
