#include "Continue.h"

#include <cmath> // TODO

#include "../sim/SimUtilities.h" // TODO: Arduino utils
#include <iostream> // TODO
#include <Milliseconds.h>

void Continue::solve(sim::MouseInterface* mouse) {
    while (true) {
        while (!wallRight(mouse)) {
            turnRight(mouse);
        }
        while (wallFront(mouse)) {
            turnLeft(mouse);
        }
        moveForward(mouse);
    }
}

bool Continue::wallRight(sim::MouseInterface* mouse) {
    return (mouse->read("C") > 0.5);
}

bool Continue::wallFront(sim::MouseInterface* mouse) {
    return (mouse->read("A") > 0.8);
}

void Continue::turnRight(sim::MouseInterface* mouse) {
    mouse->setWheelSpeeds(-10*M_PI, 10);
    sim::sleep(sim::Milliseconds(100)); // TODO: Make this part of the interface
}

void Continue::turnLeft(sim::MouseInterface* mouse) {
    mouse->setWheelSpeeds(10, 10*M_PI);
    sim::sleep(sim::Milliseconds(100));
}

void Continue::moveForward(sim::MouseInterface* mouse) {
    mouse->setWheelSpeeds(-10*M_PI, 10*M_PI);
    sim::sleep(sim::Milliseconds(100));
}
