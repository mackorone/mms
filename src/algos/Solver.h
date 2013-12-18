#ifndef SOLVER_H_
#define SOLVER_H_

#include "../sim/MouseInterface.h"

class Solver{
    
public:
    Solver(MouseInterface* mouse);
    ~Solver();
    void solve();

protected:
    MouseInterface* m_mouse;

};

#endif // SOLVER_H_
