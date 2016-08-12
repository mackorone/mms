#include "Forward.h"

namespace forward {

void Forward::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, mms::MouseInterface* mouse) {
    while (true) {
        mouse->moveForward();
    }
}

} // namespace forward
