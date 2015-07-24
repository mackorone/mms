#include "Forward.h"

namespace misc {

std::string Forward::mouseFile() const {
    return "mouse.xml";
}

sim::InterfaceType Forward::interfaceType() const {
    return sim::DISCRETE;
}

void Forward::solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) {
    while (true) {
        mouse->moveForward();
    }
}

} // namespace misc
