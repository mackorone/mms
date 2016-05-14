#include "Forward.h"

namespace forward {

void Forward::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    while (true) {
        mouse->moveForward();
    }
}

} // namespace forward
