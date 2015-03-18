#include "Solver.h"

// ------ Include all algorithm header files below this line ------- //

#include "continuous/Continue.h"
#include "discrete/wallFollow/LeftWallFollow.h"
#include "discrete/wallFollow/RightWallFollow.h"
#include "discrete/wallFollow/RandomizedWallFollow.h"
#include "discrete/floodfill/FloodFill.h"

// ------ Include all algorithm header files above this line ------- //

Solver::Solver(sim::MouseInterface* mouse) : m_mouse(mouse)
{ }

Solver::~Solver()
{ }

void Solver::solve() {

    // ------ Include all algorithm declarations below this line ------- //

    //RightWallFollow algo;
    //RandomizedWallFollow algo;
    FloodFill algo;
    //Continue algo;

    // ------ Include all algorithm declarations above this line ------- //

    algo.solve(m_mouse);
}
