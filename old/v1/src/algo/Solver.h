#pragma once

#include "../sim/MouseInterface.h"

class Solver {
    
public:
    Solver(sim::MouseInterface* mouse);
    ~Solver();
    void solve();

private:
    sim::MouseInterface* m_mouse;

};
