#pragma once

#include "../../IAlgorithm.h"

namespace demo {

class Continue : public IAlgorithm {

public:
    void solve(sim::MouseInterface* mouse);

private:
    bool wallRight();
    bool wallFront();

    void turnRight();
    void turnLeft();
    void moveForward();
    void correctErrors();

    sim::MouseInterface* m_mouse;

};

} // namespace demo
