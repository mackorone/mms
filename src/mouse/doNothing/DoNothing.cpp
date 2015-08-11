#include "DoNothing.h"

namespace doNothing {

std::string DoNothing::mouseFile() const {
    return "mouse.xml";
}

std::string DoNothing::interfaceType() const {
    return "DISCRETE";
}

void DoNothing::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {
}

} // namespace doNothing
