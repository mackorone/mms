#include "MazeAlgorithms.h"

#include "../sim/Assert.h"

#include "randomize/Randomize.h"
#include "tomasz/TomaszMazeGenerator.h"

bool MazeAlgorithms::isMazeAlgorithm(const std::string& str) {
    return helper(str, true).first;
}

IMazeAlgorithm* MazeAlgorithms::getMazeAlgorithm(const std::string& str) {
    SIM_ASSERT_TR(isMazeAlgorithm(str));
    return helper(str, false).second;
}

std::pair<bool, IMazeAlgorithm*> MazeAlgorithms::helper(const std::string& str, bool justChecking) {

    #define ALGO(NAME, INSTANCE)\
    if (str == NAME) {\
        return std::make_pair(true, justChecking ? nullptr : INSTANCE);\
    }

    ALGO("Randomize", new randomize::Randomize());
    ALGO("Tomasz", new tomasz::TomaszMazeGenerator());

    return std::make_pair(false, nullptr);
}
