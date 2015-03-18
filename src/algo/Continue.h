#pragma once

#include "IAlgorithm.h"

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
