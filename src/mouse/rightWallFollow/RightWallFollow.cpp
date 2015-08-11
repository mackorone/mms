#include "RightWallFollow.h"

namespace rightWallFollow {

std::string RightWallFollow::mouseFile() const {
    return "mouse.xml";
}

std::string RightWallFollow::interfaceType() const {
    return "DISCRETE";
}

void RightWallFollow::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {
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
