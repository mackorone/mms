#include "Manual.h"

#include <utility>

#include "../../sim/Key.h"
#include "../../sim/State.h"

namespace manual {

std::string Manual::interfaceType() const {
    return "CONTINUOUS";
}

void Manual::solve(
        int mazeWidth, int mazeHeight, bool officialMaze,
        char initialDirection, sim::MouseInterface* mouse) {

    sim::S()->setRotateZoomedMap(true);

    double accelerateAmount = 60.0;
    double decelerateAmount = 1.5;
    double leftWheelSpeed = 0.0;
    double rightWheelSpeed = 0.0;

    while (true) {

        leftWheelSpeed /= decelerateAmount;
        rightWheelSpeed /= decelerateAmount;

        if (sim::S()->arrowKeyIsPressed(sim::Key::UP)) {
            leftWheelSpeed -= accelerateAmount;
            rightWheelSpeed += accelerateAmount;
        }
        if (sim::S()->arrowKeyIsPressed(sim::Key::DOWN)) {
            leftWheelSpeed += accelerateAmount;
            rightWheelSpeed -= accelerateAmount;
        }
        if (sim::S()->arrowKeyIsPressed(sim::Key::RIGHT)) {
            leftWheelSpeed -= accelerateAmount / 4.0;
            rightWheelSpeed -= accelerateAmount / 4.0;
        }
        if (sim::S()->arrowKeyIsPressed(sim::Key::LEFT)) {
            leftWheelSpeed += accelerateAmount / 4.0;
            rightWheelSpeed += accelerateAmount / 4.0;
        }

        if (leftWheelSpeed < -mouse->getWheelMaxSpeed("left")) {
            leftWheelSpeed = -mouse->getWheelMaxSpeed("left");    
        }
        if (mouse->getWheelMaxSpeed("left") < leftWheelSpeed) {
            leftWheelSpeed = mouse->getWheelMaxSpeed("left");
        }
        if (rightWheelSpeed < -mouse->getWheelMaxSpeed("right")) {
            rightWheelSpeed = -mouse->getWheelMaxSpeed("right");    
        }
        if (mouse->getWheelMaxSpeed("right") < rightWheelSpeed) {
            rightWheelSpeed = mouse->getWheelMaxSpeed("right");
        }

        mouse->setWheelSpeed("left", leftWheelSpeed);
        mouse->setWheelSpeed("right", rightWheelSpeed);
        mouse->delay(30);
    }
}

} // namespace misc
