#include "MouseAlgorithms.h"

#include "../sim/Assert.h"

#include "continuous/Continuous.h"
#include "doNothing/DoNothing.h"
#include "floodFill/FloodFill.h"
#include "fontTest/FontTest.h"
#include "forward/Forward.h"
#include "leftWallFollow/LeftWallFollow.h"
#include "mackAlgo/MackAlgo.h"
#include "mackAlgoTwo/MackAlgoTwo.h"
#include "manual/Manual.h"
#include "randomizedWallFollow/RandomizedWallFollow.h"
#include "rightWallFollow/RightWallFollow.h"
#include "test/Test.h"

bool MouseAlgorithms::isMouseAlgorithm(const std::string& str) {
    return helper(str, true).first;
}

IMouseAlgorithm* MouseAlgorithms::getMouseAlgorithm(const std::string& str) {
    SIM_ASSERT_TR(isMouseAlgorithm(str));
    return helper(str, false).second;
}

std::pair<bool, IMouseAlgorithm*> MouseAlgorithms::helper(const std::string& str, bool justChecking) {

    #define ALGO(NAME, INSTANCE)\
    if (str == NAME) {\
        return std::make_pair(true, justChecking ? nullptr : INSTANCE);\
    }

    ALGO("Continuous", new continuous::Continuous());
    ALGO("DoNothing", new doNothing::DoNothing());
    ALGO("FloodFill", new floodFill::FloodFill());
    ALGO("FontTest", new fontTest::FontTest());
    ALGO("Forward", new forward::Forward());
    ALGO("LeftWallFollow", new leftWallFollow::LeftWallFollow());
    ALGO("MackAlgo", new mackAlgo::MackAlgo());
    ALGO("MackAlgoTwo", new mackAlgoTwo::MackAlgoTwo());
    ALGO("Manual", new manual::Manual());
    ALGO("RandomizedWallFollow", new randomizedWallFollow::RandomizedWallFollow());
    ALGO("RightWallFollow", new rightWallFollow::RightWallFollow());
    ALGO("Test", new test::Test());

    return std::make_pair(false, nullptr);
}
