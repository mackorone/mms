#pragma once

#include "../IAlgorithm.h"

namespace manual {

class Manual : public IAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

};

} // namespace misc
