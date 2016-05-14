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
    return "omniMouse.xml";
}

std::string Test::interfaceType() const {
    return "CONTINUOUS";
}

void Test::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    // This causes translation
    mouse->setWheelSpeed("upper-left", -50);
    mouse->setWheelSpeed("upper-right", -50);
    mouse->setWheelSpeed("bottom", 100);
    while (true) {
        mouse->delay(100);
    }
}

} // namespace test
