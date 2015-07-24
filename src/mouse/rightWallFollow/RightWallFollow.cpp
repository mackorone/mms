#include "RightWallFollow.h"

namespace rightWallFollow {

std::string RightWallFollow::mouseFile() const {
    return "mouse.xml";
}

sim::InterfaceType RightWallFollow::interfaceType() const {
    return sim::DISCRETE;
}

void RightWallFollow::solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse) {
    while (true){
        rightWallFollowStep(mouse);
    }
}

void RightWallFollow::rightWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallRight()){
        mouse->turnRight();
    }
    while (mouse->wallFront()){
        mouse->turnLeft();
    }
    mouse->moveForward();
}

} // namespace rightWallFollow
