#include "Wait.h"

namespace misc {

std::string Wait::mouseFile() const {
    return "mouse.xml";
}

sim::InterfaceType Wait::interfaceType() const {
    return sim::DISCRETE;
}

void Wait::solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) {
}

} // namespace misc
