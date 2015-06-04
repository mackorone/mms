#include "Forward.h"

namespace misc {

void Forward::solve() {
    M()->initializeMouse("mouse.xml");
    M()->declareInterfaceType(sim::DISCRETE);
    while (true) {
        M()->moveForward();
    }
}

} // namespace misc
