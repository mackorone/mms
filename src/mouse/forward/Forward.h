#pragma once

#include "../IMouseAlgorithm.h"

namespace forward {

class Forward : public IMouseAlgorithm {

public:
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

};

} // namespace forward
