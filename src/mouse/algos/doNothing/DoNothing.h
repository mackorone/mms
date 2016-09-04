#pragma once

#include "../IMouseAlgorithm.h"

namespace doNothing {

class DoNothing : public IMouseAlgorithm {

public:
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, mms::MouseInterface* mouse);

};

} // namespace doNothing
