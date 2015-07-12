#pragma once

#include "../IAlgorithm.h"

namespace manual {

class Manual : public IAlgorithm {

public:
    void solve(sim::MouseInterface* mouse);

};

} // namespace misc
