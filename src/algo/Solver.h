#ifndef SOLVER_H_
#define SOLVER_H_

#include "../sim/MouseInterface.h"

class Solver {
    
public:
    Solver(sim::MouseInterface* mouse);
    ~Solver();
    void solve();

private:
    sim::MouseInterface* m_mouse;

};

#endif // SOLVER_H_
