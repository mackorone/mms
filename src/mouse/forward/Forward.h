#pragma once

#include "../IMouseAlgorithm.h"

namespace forward {

class Forward : public IMouseAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

};

} // namespace forward
