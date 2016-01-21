#include "RightWallFollow.h"

namespace rightWallFollow {

std::string RightWallFollow::mouseFile() const {
    return "omniMouse.xml";
}

bool RightWallFollow::declareWallOnRead() const {
    return true;
}

bool RightWallFollow::declareBothWallHalves() const {
    return true;
}

bool RightWallFollow::stopOnTileEdgesAndAllowSpecialMovements() const {
    return true;
}

void RightWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    while (true){
        rightWallFollowStep(mouse);
    }
}

void RightWallFollow::rightWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallRight()) {
        turnRightAndMoveForward(mouse);
    }
    else if (!mouse->wallFront()) {
        mouse->moveForward();
    }
    else if (!mouse->wallLeft()) {
        turnLeftAndMoveForward(mouse);
    }
    else {
        turnAroundAndMoveForward(mouse);
    }
}

void RightWallFollow::turnLeftAndMoveForward(sim::MouseInterface* mouse) {
    if (stopOnTileEdgesAndAllowSpecialMovements()) {
        mouse->curveTurnLeft();
    }
    else {
        mouse->turnLeft();
        mouse->moveForward();
    }
}

void RightWallFollow::turnRightAndMoveForward(sim::MouseInterface* mouse) {
    if (stopOnTileEdgesAndAllowSpecialMovements()) {
        mouse->curveTurnRight();
    }
    else {
        mouse->turnRight();
        mouse->moveForward();
    }
}

void RightWallFollow::turnAroundAndMoveForward(sim::MouseInterface* mouse) {
    mouse->turnAroundLeft();
    if (!stopOnTileEdgesAndAllowSpecialMovements()) {
        mouse->moveForward();
    }
}

} // namespace rightWallFollow
