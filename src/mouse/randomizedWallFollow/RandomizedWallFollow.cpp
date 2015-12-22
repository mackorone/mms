#include "RandomizedWallFollow.h"

#include <cstdlib>

namespace randomizedWallFollow {

bool RandomizedWallFollow::declareWallOnRead() const {
    return true;
}

bool RandomizedWallFollow::declareBothWallHalves() const {
    return true;
}

void RandomizedWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    while (true){
        if (rand() % 2 == 0){
            rightWallFollowStep(mouse);
        }
        else{
            leftWallFollowStep(mouse);
        }
    }
}

void RandomizedWallFollow::rightWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallRight()){
        mouse->turnRight();
    }
    while (mouse->wallFront()){
        mouse->turnLeft();
    }
    mouse->moveForward();
}

void RandomizedWallFollow::leftWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}

} // namespace randomizedWallFollow
