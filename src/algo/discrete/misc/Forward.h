#pragma once

#include "../../IAlgorithm.h"

namespace misc {

class Forward : public IAlgorithm {

public:
    Forward(sim::MouseInterface* mouse) : IAlgorithm(mouse) {}
    void solve();

};

} // namespace misc
