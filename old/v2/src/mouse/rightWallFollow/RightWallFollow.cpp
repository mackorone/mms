#include "RightWallFollow.h"

namespace rightWallFollow {

std::string RightWallFollow::mouseFile() const {
    // The options here are:
    // - "default.xml"
    // - "megaMouse.xml"
    // - "omniMouse.xml"
    return "omniMouse.xml";
}

bool RightWallFollow::declareWallOnRead() const {
    // This makes it easy to see the walls the mouse has inspected
    return true;
}

bool RightWallFollow::declareBothWallHalves() const {
    // Ensures that we delcare both halves of any wall we declare
    return true;
}

bool RightWallFollow::useTileEdgeMovements() const {
    // If false, the mouse will use basic movements
    // If true, it will use edge movements that allow for faster maze traversal
    return true;
}

double RightWallFollow::wheelSpeedFraction() const {
    // The max speed of the wheels of the megaMouse is too great for DISCRETE
    // mode, so here we ensure that they operate at a reasonable speed, i.e.,
    // 1/20th of their max
    if (mouseFile() == "megaMouse.xml") {
        return 1.0 / 20.0;
    }
    return 1.0;
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
    if (useTileEdgeMovements()) {
        // If we're using special tile edge movements, then the origin is a
        // special case, since we don't start on a tile edge.
        static bool firstMovement = true;
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
