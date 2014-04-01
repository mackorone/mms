#include "Solver.h"

// ------ Include all algorithm header files below this line ------- //

#include "wallFollow/LeftWallFollow.h"
#include "wallFollow/RightWallFollow.h"
#include "wallFollow/RandomizedWallFollow.h"
#include "Mack_Ward/FloodFill/FloodFill.h"

// ------ Include all algorithm header files above this line ------- //

Solver::Solver(sim::MouseInterface* mouse) : m_mouse(mouse)
{ }

Solver::~Solver()
{ }

void Solver::solve(){

    // ------ Include all algorithm declarations below this line ------- //

    //LeftWallFollow algo;
    //RightWallFollow algo;
    //RandomizedWallFollow algo;
    FloodFill algo;

    // ------ Include all algorithm declarations above this line ------- //

    algo.solve(m_mouse);
}
