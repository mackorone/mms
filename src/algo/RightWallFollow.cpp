#include "RightWallFollow.h"

void RightWallFollow::solve(MouseInterface* mouse){
    while (true){
        rightWallFollowStep(mouse);
    }
}

void RightWallFollow::rightWallFollowStep(MouseInterface* mouse){
    if (!mouse->wallRight()){
        mouse->turnRight();
    }
    while (mouse->wallFront()){
        mouse->turnLeft();
    }
    mouse->moveForward();
}
