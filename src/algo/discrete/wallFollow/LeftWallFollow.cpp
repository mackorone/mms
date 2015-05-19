#include "LeftWallFollow.h"

namespace wallFollow {

void LeftWallFollow::solve(sim::MouseInterface* mouse){
    mouse->initializeMouse("mouse.xml");
    mouse->declareInterfaceType(sim::DISCRETE);
    while (true) {
        leftWallFollowStep(mouse);
    }
}

void LeftWallFollow::leftWallFollowStep(sim::MouseInterface* mouse){
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}

} // namespace wallFollow
