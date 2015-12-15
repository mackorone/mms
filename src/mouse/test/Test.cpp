#include "Test.h"

#include <iostream>
#ifdef _WIN32
#   include <Windows.h>
#   define _USE_MATH_DEFINES
#   undef max
#   undef min
#	include <Windows.h>
#endif
#include <iostream>
#include <cmath>
#include <ctime>
#undef M_PI
#define M_PI 3.14159265358979323846

namespace test {

std::string Test::mouseFile() const {
    return "megaMouse.xml";
}

std::string Test::interfaceType() const {
    return "CONTINUOUS";
}

void Test::solve(
        int mazeWidth, int mazeHeight, bool officialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    mouse->setWheelSpeed("left-lower", -100);
    mouse->setWheelSpeed("left-upper", -100);
    mouse->setWheelSpeed("right-lower", 100);
    mouse->setWheelSpeed("right-upper", 100);
    while (true) {
        mouse->delay(100);
    }
}

} // namespace test
