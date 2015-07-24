#include "LeftWallFollow.h"

namespace leftWallFollow {

std::string LeftWallFollow::mouseFile() const {
    return "mouse.xml";
}

sim::InterfaceType LeftWallFollow::interfaceType() const {
    return sim::DISCRETE;
}

void LeftWallFollow::solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) {
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
