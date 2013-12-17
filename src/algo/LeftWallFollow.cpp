#include "LeftWallFollow.h"

void LeftWallFollow::solve(MouseInterface* mouse){
    while (true){
        leftWallFollowStep(mouse);
    }
}

void LeftWallFollow::leftWallFollowStep(MouseInterface* mouse){
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}
