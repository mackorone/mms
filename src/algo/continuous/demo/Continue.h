#pragma once

#include "../../IAlgorithm.h"

namespace demo {

class Continue : public IAlgorithm {

public:
    Continue(sim::MouseInterface* mouse) : IAlgorithm(mouse) {}
    void solve();

private:
    bool wallRight();
    bool wallFront();

    void turnRight();
    void turnLeft();
    void moveForward();
    void correctErrors();

};

} // namespace demo
