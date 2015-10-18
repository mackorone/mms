#include "MouseAlgorithms.h"

#include "../sim/Assert.h"

#include "manual/Manual.h"
#include "continuous/Continuous.h"
#include "doNothing/DoNothing.h"
#include "forward/Forward.h"
#include "leftWallFollow/LeftWallFollow.h"
#include "rightWallFollow/RightWallFollow.h"
#include "randomizedWallFollow/RandomizedWallFollow.h"
#include "floodFill/FloodFill.h"
#include "mackAlgo/MackAlgo.h"

bool MouseAlgorithms::isMouseAlgorithm(const std::string& str) {
    return helper(str, true).first;
}

IMouseAlgorithm* MouseAlgorithms::getMouseAlgorithm(const std::string& str) {
    ASSERT_TRUE(isMouseAlgorithm(str));
    return helper(str, false).second;
}

std::pair<bool, IMouseAlgorithm*> MouseAlgorithms::helper(const std::string& str, bool justChecking) {

    #define ALGO(NAME, INSTANCE)\
    if (str == NAME) {\
        return std::make_pair(true, justChecking ? nullptr : INSTANCE);\
    }

    ALGO("Manual", new manual::Manual());
    ALGO("Continuous", new continuous::Continuous());
    ALGO("DoNothing", new doNothing::DoNothing());
    ALGO("Forward", new forward::Forward());
    ALGO("LeftWallFollow", new leftWallFollow::LeftWallFollow());
    ALGO("RightWallFollow", new rightWallFollow::RightWallFollow());
    ALGO("RandomizedWallFollow", new randomizedWallFollow::RandomizedWallFollow());
    ALGO("FloodFill", new floodFill::FloodFill());
    ALGO("MackAlgo", new mackAlgo::MackAlgo());

    return std::make_pair(false, nullptr);
}
