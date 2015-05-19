#include "Wait.h"

namespace misc {

void Wait::solve(sim::MouseInterface* mouse) {
    mouse->initializeMouse("mouse.xml");
    mouse->declareInterfaceType(sim::DISCRETE);
}

} // namespace misc
