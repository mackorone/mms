#include "MouseInterface.h"

namespace mack {

MouseInterface::MouseInterface() {
}

bool MouseInterface::inputButtonPressed(int inputButton) {
    // TODO : Kyle, put code here that checks to see if the button-pressed flag was set
}

void MouseInterface::acknowledgeInputButtonPressed(int inputButton) {
    // TODO : Kyle, put code here that resets the button-pressed flags
}

bool MouseInterface::wallFront() {
    // TODO : Kyle, put code here that tells me (as quickly as possible) if there's a wall in front
}

bool MouseInterface::wallRight() {
    // TODO : Kyle, put code here that tells me (as quickly as possible) if there's a wall to the right
}

bool MouseInterface::wallLeft() {
    // TODO : Kyle, put code here that tells me (as quickly as possible) if there's a wall to the left
}

void MouseInterface::moveForward() {
    // TODO : Kyle, put code here that tells the mouse to move forward
}

void MouseInterface::turnRight() {
    // TODO : Kyle, this won't work quite yet - I haven't implemented curve turns yet
}

void MouseInterface::turnLeft() {
    // TODO : Kyle, this won't work quite yet - I haven't implemented curve turns yet
}

void MouseInterface::turnAround() {
    // TODO : Kyle, this won't work quite yet - I haven't implemented turn-around-and-move-forward
}

} // namespace mack
