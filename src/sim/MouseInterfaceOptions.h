#pragma once

#include "InterfaceType.h"

class IMouseAlgorithm;

namespace sim {

struct MouseInterfaceOptions {
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    InterfaceType interfaceType;
};

} // namespace sim
