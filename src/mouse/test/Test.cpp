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
    return "default.xml";
}

std::string Test::interfaceType() const {
    return "CONTINUOUS";
}

void Test::solve(
        int mazeWidth, int mazeHeight, bool officialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    mouse->setWheelSpeed("left", -10);
    mouse->setWheelSpeed("right", 10);
    while (true) {
        if (mouse->inputButtonPressed(0)) {
            mouse->resetWheelEncoder("left");
            mouse->resetWheelEncoder("right");
            mouse->acknowledgeInputButtonPressed(0);
        }
        int left = mouse->readWheelEncoder("left");
        int right = mouse->readWheelEncoder("right");
        mouse->info(std::string("L: ") + std::to_string(left));
        mouse->info(std::string("R: ") + std::to_string(right));
        mouse->delay(100);
    }
}

} // namespace test
