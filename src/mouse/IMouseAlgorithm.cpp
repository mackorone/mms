#include "IMouseAlgorithm.h"

std::string IMouseAlgorithm::mouseFile() const {
    return "default.xml";
}

std::string IMouseAlgorithm::interfaceType() const {
    return "DISCRETE";
}

std::string IMouseAlgorithm::initialDirection() const {
    return "NORTH";
}

bool IMouseAlgorithm::controlTileFog() const {
    return false;
}

bool IMouseAlgorithm::declareBothWallHalves() const {
    return false;
}

bool IMouseAlgorithm::declareWallOnRead() const {
    return false;
}

bool IMouseAlgorithm::setTileBaseColorWhenDistanceCorrect() const {
    return false;
}

double IMouseAlgorithm::wheelSpeedFraction() const {
    return 1.0; 
}
