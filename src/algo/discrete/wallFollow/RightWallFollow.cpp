#include "RightWallFollow.h"

namespace wallFollow {

void RightWallFollow::solve(){
    M()->initializeMouse("mouse.xml");
    M()->declareInterfaceType(sim::DISCRETE);
    while (true){
        rightWallFollowStep();
    }
}

void RightWallFollow::rightWallFollowStep(){
    if (!M()->wallRight()){
        M()->turnRight();
    }
    while (M()->wallFront()){
        M()->turnLeft();
    }
    M()->moveForward();
}

} // namespace wallFollow
