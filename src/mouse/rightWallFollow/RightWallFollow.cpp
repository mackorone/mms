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

bool RightWallFollow::useTileEdgeMovements() const {
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
    // TODO: MACK - testing diagonals
    justMoveForward(mouse);
    turnRightAndMoveForward(mouse);
    turnRightAndMoveForward(mouse);
    turnLeftAndMoveForward(mouse);
    for (int i = 0; i < 10; i += 1) {
        justMoveForward(mouse);
    }
    mouse->turnAroundLeftToEdge();
    mouse->diagonalRightRight(3);
    mouse->diagonalRightRight(5);
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
        justMoveForward(mouse);
    }
    else if (!mouse->wallLeft()) {
        turnLeftAndMoveForward(mouse);
    }
    else {
        turnAroundAndMoveForward(mouse);
    }
}

void RightWallFollow::justMoveForward(sim::MouseInterface* mouse) {
    static bool firstMovement = true;
    if (useTileEdgeMovements()) {
        if (firstMovement) {
            mouse->originMoveForwardToEdge();
            firstMovement = false;    
        }
        else {
            mouse->moveForwardToEdge();
        }
        
    }
    else {
        mouse->moveForward();
    }
}

void RightWallFollow::turnLeftAndMoveForward(sim::MouseInterface* mouse) {
    if (useTileEdgeMovements()) {
        mouse->turnLeftToEdge();
    }
    else {
        mouse->turnLeft();
        mouse->moveForward();
    }
}

void RightWallFollow::turnRightAndMoveForward(sim::MouseInterface* mouse) {
    if (useTileEdgeMovements()) {
        mouse->turnRightToEdge();
    }
    else {
        mouse->turnRight();
        mouse->moveForward();
    }
}

void RightWallFollow::turnAroundAndMoveForward(sim::MouseInterface* mouse) {
    if (useTileEdgeMovements()) {
        mouse->turnAroundLeftToEdge();
    }
    else {
        mouse->turnAroundLeft();
        mouse->moveForward();
    }
}

} // namespace rightWallFollow
