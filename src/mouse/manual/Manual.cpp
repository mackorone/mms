#include "Manual.h"

#include <utility>

#include "../../sim/Directions.h"
#include "../../sim/Keys.h"
#include "../../sim/State.h"

namespace manual {

std::string Manual::mouseFile() const {
    return "mouse.xml";
}

sim::InterfaceType Manual::interfaceType() const {
    return sim::CONTINUOUS;
}

void Manual::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {

    sim::S()->setRotateZoomedMap(true);

    double accelerateAmount = 5.0;
    double decelerateAmount = 1.2;
    double leftWheelSpeed = 0.0;
    double rightWheelSpeed = 0.0;

    std::map<char, sim::Direction> directionsMap;
    directionsMap.insert(std::make_pair('n', sim::NORTH));
    directionsMap.insert(std::make_pair('e', sim::EAST));
    directionsMap.insert(std::make_pair('s', sim::SOUTH));
    directionsMap.insert(std::make_pair('w', sim::WEST));

    while (true) {

        std::pair<int, int> location = mouse->getDiscretizedTranslation();
        mouse->setTileFogginess(location.first, location.second, false);
        for (std::pair<char, sim::Direction> pair : directionsMap) {
            mouse->declareWall(location.first, location.second, pair.first,
                mouse->isWall(location, pair.second));
        }

        leftWheelSpeed /= decelerateAmount;
        rightWheelSpeed /= decelerateAmount;

        if (sim::S()->arrowKeyIsPressed(sim::UP)) {
            leftWheelSpeed -= accelerateAmount;
            rightWheelSpeed += accelerateAmount;
        }
        if (sim::S()->arrowKeyIsPressed(sim::DOWN)) {
            leftWheelSpeed += accelerateAmount;
            rightWheelSpeed -= accelerateAmount;
        }
        if (sim::S()->arrowKeyIsPressed(sim::RIGHT)) {
            leftWheelSpeed -= accelerateAmount / 4.0;
            rightWheelSpeed -= accelerateAmount / 4.0;
        }
        if (sim::S()->arrowKeyIsPressed(sim::LEFT)) {
            leftWheelSpeed += accelerateAmount / 4.0;
            rightWheelSpeed += accelerateAmount / 4.0;
        }

        mouse->setWheelSpeeds(leftWheelSpeed, rightWheelSpeed);
        mouse->delay(30);
    }
}

} // namespace misc
