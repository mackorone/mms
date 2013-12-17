#include "RandomizedWallFollow.h"

void RandomizedWallFollow::solve(MouseInterface* mouse){
    while (true){
        if (rand() % 2 == 0){
            rightWallFollowStep(mouse);
        }
        else{
            leftWallFollowStep(mouse);
        }
    }
}

void RandomizedWallFollow::rightWallFollowStep(MouseInterface* mouse){
    if (!mouse->wallRight()){
        mouse->turnRight();
    }
    while (mouse->wallFront()){
        mouse->turnLeft();
    }
    mouse->moveForward();
}

void RandomizedWallFollow::leftWallFollowStep(MouseInterface* mouse){
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}
