#include "Manual.h"

#include <utility>

#include "../../sim/Direction.h"
#include "../../sim/Key.h"
#include "../../sim/State.h"

namespace manual {

std::string Manual::mouseFile() const {
    return "mouse.xml";
}

std::string Manual::interfaceType() const {
    return "CONTINUOUS";
}

void Manual::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {

    sim::S()->setRotateZoomedMap(true);

    double accelerateAmount = 5.0;
    double decelerateAmount = 1.2;
    double leftWheelSpeed = 0.0;
    double rightWheelSpeed = 0.0;

    std::map<char, sim::Direction> directionsMap;
    directionsMap.insert(std::make_pair('n', sim::Direction::NORTH));
    directionsMap.insert(std::make_pair('e', sim::Direction::EAST));
    directionsMap.insert(std::make_pair('s', sim::Direction::SOUTH));
    directionsMap.insert(std::make_pair('w', sim::Direction::WEST));

    while (true) {

        std::pair<int, int> location = mouse->getDiscretizedTranslation();
        mouse->setTileFogginess(location.first, location.second, false);
        for (std::pair<char, sim::Direction> pair : directionsMap) {
            mouse->declareWall(location.first, location.second, pair.first,
                mouse->isWall(location, pair.second));
        }

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

        mouse->setWheelSpeeds(leftWheelSpeed, rightWheelSpeed);
        mouse->delay(30);
    }
}

} // namespace misc
