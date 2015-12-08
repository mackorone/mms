#include "IMouseAlgorithm.h"

std::string IMouseAlgorithm::mouseFile() const {
    return "default.xml";
}

std::string IMouseAlgorithm::interfaceType() const {
    return "DISCRETE";
}

double IMouseAlgorithm::wheelSpeedFraction() const {
    return 1.0; 
}
