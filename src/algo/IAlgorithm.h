#pragma once

#include "../sim/InterfaceType.h"
#include "../sim/MouseInterface.h"

class IAlgorithm {

public:
    IAlgorithm(sim::MouseInterface* mouse);
    sim::MouseInterface* M();
    virtual void solve() = 0;

private:
    sim::MouseInterface* m_mouse;

};
