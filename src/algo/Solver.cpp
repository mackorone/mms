#include "Solver.h"

#include "continuous/demo/Continue.h"
#include "discrete/floodfill/FloodFill.h"
#include "discrete/mack/MackAlgo.h"
#include "discrete/misc/Forward.h"
#include "discrete/misc/Wait.h"
#include "discrete/wallFollow/LeftWallFollow.h"
#include "discrete/wallFollow/RightWallFollow.h"
#include "discrete/wallFollow/RandomizedWallFollow.h"

Solver::Solver(sim::MouseInterface* mouse) : m_mouse(mouse) {
}

void Solver::solve() {

    // TODO: Put these into a map, let the algo be decided at runtime
    //demo::Continue algo;
    //floodfill::FloodFill algo;
    //mack::MackAlgo algo;
    //misc::Forward algo;
    //misc::Wait algo;
    //wallFollow::RightWallFollow algo;
    //wallFollow::LeftWallFollow algo;
    //wallFollow::RandomizedWallFollow algo;

    algo.solve(m_mouse);
}
