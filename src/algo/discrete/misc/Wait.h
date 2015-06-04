#pragma once

#include "../../IAlgorithm.h"

namespace misc {

class Wait : public IAlgorithm {

public:
    Wait(sim::MouseInterface** mouse) : IAlgorithm(mouse) {}
    void solve();

};

} // namespace misc
