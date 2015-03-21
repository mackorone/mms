#pragma once

#include "../../IAlgorithm.h"

namespace mack {

class MackAlgo : IAlgorithm {

// TODO: All paths w/o a cycle to the center - determine the shortest with the edge weights

public:
    void solve(sim::MouseInterface* mouse);

private:
    sim::MouseInterface* m_mouse;

};

} // namespace mack
