#include "FontTest.h"

#include "../../sim/Key.h"
#include "../../sim/State.h"

namespace fontTest {

std::string FontTest::interfaceType() const {
    return "CONTINUOUS";
}

void FontTest::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {

    // Put all the characters on the closest tiles

    std::string allChars =
        " !\"#$%&'()*+,-./0123456789:;<=>?"
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
        "`abcdefghijklmnopqrstuvwxyz{|}~";

    for (int i = 0; i < 6; i += 1) {
        for (int j = 0; j < 6; j += 1) {
            int index = 18 * i + 3 * j + 0;
            std::string str;
            for (int k = 0; k < 3; k += 1) {
                if (index + k < allChars.size()) {
                    str += allChars.substr(index + k, 1);
                    str += " ";
                }
            }
            mouse->setTileText(i, 5 - j, str);
        }
    }

    // Easy manual mode control so that we can look around

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

} // namespace fontTest
