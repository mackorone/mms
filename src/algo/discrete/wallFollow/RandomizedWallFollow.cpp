#include "RandomizedWallFollow.h"

#include <cstdlib>

namespace wallFollow {

void RandomizedWallFollow::solve(){
    M()->initializeMouse("mouse.xml");
    M()->declareInterfaceType(sim::DISCRETE);
    while (true){
        if (rand() % 2 == 0){
            rightWallFollowStep();
        }
        else{
            leftWallFollowStep();
        }
    }
}

void RandomizedWallFollow::rightWallFollowStep(){
    if (!M()->wallRight()){
        M()->turnRight();
    }
    while (M()->wallFront()){
        M()->turnLeft();
    }
    M()->moveForward();
}

void RandomizedWallFollow::leftWallFollowStep(){
    if (!M()->wallLeft()){
        M()->turnLeft();
    }
    while (M()->wallFront()){
        M()->turnRight();
    }
    M()->moveForward();
}

} // namespace wallFollow
