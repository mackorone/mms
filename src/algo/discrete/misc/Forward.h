#pragma once

#include "../../IAlgorithm.h"

namespace misc {

class Forward : public IAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

};

} // namespace misc
