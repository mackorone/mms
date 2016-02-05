#include "RightWallFollow.h"

namespace rightWallFollow {

std::string RightWallFollow::mouseFile() const {
    return "default.xml";
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

double RightWallFollow::wheelSpeedFraction() const {
    if (mouseFile() == "megaMouse.xml") {
        return 0.05;
    }
    return 1.0;
}

void RightWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    mouse->moveForward();
    turnRightAndMoveForward(mouse);
    turnRightAndMoveForward(mouse);
    turnLeftAndMoveForward(mouse);
    for (int i = 0; i < 10; i += 1) {
        mouse->moveForward();
    }
    mouse->turnAroundLeft();
    mouse->diagonalRightRight(3);
    mouse->diagonalRightRight(5);
    // TODO: MACK
    /*
    while (true){
        rightWallFollowStep(mouse);
    }
    */
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
