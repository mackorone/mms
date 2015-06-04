#include "Continue.h"

#include <cmath>

namespace demo {

void Continue::solve() {

    M()->initializeMouse("megaMouse.xml");
    M()->declareInterfaceType(sim::CONTINUOUS);

    while (true) {
        if (!wallRight()) {
            turnRight();
        }
        while (wallFront()) {
            turnLeft();
        }
        moveForward();
        correctErrors();
    }
}

bool Continue::wallRight() {
    return M()->read("right") > 0.5;
}

bool Continue::wallFront() {
    return M()->read("rightFront") > 0.8;
}

void Continue::turnRight() {
    M()->setWheelSpeeds(-5*M_PI, -5*M_PI);
    M()->delay(290);
    M()->setWheelSpeeds(0, 0);
}

void Continue::turnLeft() {
    M()->setWheelSpeeds(5*M_PI, 5*M_PI);
    M()->delay(290);
    M()->setWheelSpeeds(0, 0);
}

void Continue::moveForward() {
    M()->setWheelSpeeds(-10*M_PI, 10*M_PI);
    M()->delay(280);
    for (int i = 0; i < 10; i += 1) {
        if (M()->read("rightFront") > 0.95) {
            break;
        }
        M()->delay(10);
    }
    M()->setWheelSpeeds(0, 0);
    M()->delay(0);
}

void Continue::correctErrors() {
    if (M()->read("rightMiddle") < 0.65) {
        // Right
        M()->setWheelSpeeds(-5, -5);
        M()->delay(10);
    }
    if (M()->read("rightMiddle") > 0.8) {
        // Left
        M()->setWheelSpeeds(5, 5);
        M()->delay(10);
    }
}

} // namespace demo
