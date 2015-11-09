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

void Test::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {
    mouse->setWheelSpeed("left", -10);
    mouse->setWheelSpeed("right", 10);
}

} // namespace test
