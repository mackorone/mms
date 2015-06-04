#include "Wait.h"

namespace misc {

void Wait::solve() {
    M()->initializeMouse("mouse.xml");
    M()->declareInterfaceType(sim::DISCRETE);
}

} // namespace misc
