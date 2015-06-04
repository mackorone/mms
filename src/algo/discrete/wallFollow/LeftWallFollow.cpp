#include "LeftWallFollow.h"

namespace wallFollow {

void LeftWallFollow::solve() {
    M()->initializeMouse("mouse.xml");
    M()->declareInterfaceType(sim::DISCRETE);
    while (true) {
        leftWallFollowStep();
    }
}

void LeftWallFollow::leftWallFollowStep() {
    if (!M()->wallLeft()){
        M()->turnLeft();
    }
    while (M()->wallFront()){
        M()->turnRight();
    }
    M()->moveForward();
}

} // namespace wallFollow
