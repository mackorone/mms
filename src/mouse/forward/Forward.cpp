#include "Forward.h"

namespace forward {

std::string Forward::mouseFile() const {
    return "mouse.xml";
}

std::string Forward::interfaceType() const {
    return "DISCRETE";
}

void Forward::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {
    while (true) {
        mouse->moveForward();
    }
}

} // namespace forward
