#include "Forward.h"

namespace misc {

void Forward::solve(sim::MouseInterface* mouse) {
    mouse->declareInterfaceType(sim::DISCRETE);
    while (true) {
        mouse->moveForward();
    }
}

} // namespace misc
