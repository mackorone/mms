#include "LeftWallFollow.h"

namespace leftWallFollow {

bool LeftWallFollow::declareWallOnRead() const {
    return true;
}

bool LeftWallFollow::declareBothWallHalves() const {
    return true;
}

void LeftWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    while (true) {
        leftWallFollowStep(mouse);
    }
}

void LeftWallFollow::leftWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}

} // namespace leftWallFollow
