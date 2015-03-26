#include "Solver.h"

// ------ Include all algorithm header files below this line ------- //

#include "continuous/demo/Continue.h"
#include "discrete/floodfill/FloodFill.h"
//#include "discrete/mack/MackAlgo.h"
#include "discrete/misc/Forward.h"
#include "discrete/wallFollow/LeftWallFollow.h"
#include "discrete/wallFollow/RightWallFollow.h"
#include "discrete/wallFollow/RandomizedWallFollow.h"

// ------ Include all algorithm header files above this line ------- //

Solver::Solver(sim::MouseInterface* mouse) : m_mouse(mouse)
{ }

Solver::~Solver()
{ }

void Solver::solve() {

    // ------ Include all algorithm declarations below this line ------- //

    //demo::Continue algo;
    floodfill::FloodFill algo;
    //mack::MackAlgo algo;
    //misc::Forward algo;
    //wallFollow::RightWallFollow algo;
    //wallFollow::RandomizedWallFollow algo;

    // ------ Include all algorithm declarations above this line ------- //

    algo.solve(m_mouse);
}
