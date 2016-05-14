#include "Manual.h"

#include "../../sim/Key.h"
#include "../../sim/State.h"

namespace manual {

std::string Manual::mouseFile() const {
    return "megaMouse.xml";
}

std::string Manual::interfaceType() const {
    return "CONTINUOUS";
}

void Manual::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
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

        if (mouseFile() == "megaMouse.xml") {
            if (leftWheelSpeed < -mouse->getWheelMaxSpeed("left-upper")) {
                leftWheelSpeed = -mouse->getWheelMaxSpeed("left-upper");    
            }
            if (mouse->getWheelMaxSpeed("left-upper") < leftWheelSpeed) {
                leftWheelSpeed = mouse->getWheelMaxSpeed("left-upper");
            }
            if (rightWheelSpeed < -mouse->getWheelMaxSpeed("right-upper")) {
                rightWheelSpeed = -mouse->getWheelMaxSpeed("right-upper");    
            }
            if (mouse->getWheelMaxSpeed("right-upper") < rightWheelSpeed) {
                rightWheelSpeed = mouse->getWheelMaxSpeed("right-upper");
            }

            mouse->setWheelSpeed("left-upper", leftWheelSpeed);
            mouse->setWheelSpeed("left-lower", leftWheelSpeed);
            mouse->setWheelSpeed("right-upper", rightWheelSpeed);
            mouse->setWheelSpeed("right-lower", rightWheelSpeed);
        }

        else if (mouseFile() == "default.xml") {
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
        }

        mouse->delay(30);
    }
}

} // namespace manual
