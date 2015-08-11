#include "DoNothing.h"

namespace doNothing {

std::string DoNothing::mouseFile() const {
    return "mouse.xml";
}

sim::InterfaceType DoNothing::interfaceType() const {
    return sim::InterfaceType::DISCRETE;
}

void DoNothing::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {
}

} // namespace doNothing
