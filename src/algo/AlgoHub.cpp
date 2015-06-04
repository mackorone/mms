#include "AlgoHub.h"

#include <map>

#include "continuous/demo/Continue.h"
#include "discrete/floodfill/FloodFill.h"
#include "discrete/mack/MackAlgo.h"
#include "discrete/misc/Forward.h"
#include "discrete/misc/Wait.h"
#include "discrete/wallFollow/LeftWallFollow.h"
#include "discrete/wallFollow/RightWallFollow.h"
#include "discrete/wallFollow/RandomizedWallFollow.h"

AlgoHub::AlgoHub(sim::MouseInterface* mouse) {
    m_algorithms.insert(std::make_pair("Continue", new demo::Continue(mouse)));
    m_algorithms.insert(std::make_pair("FloodFill", new floodfill::FloodFill(mouse)));
    m_algorithms.insert(std::make_pair("MackAlgo", new mack::MackAlgo(mouse)));
    m_algorithms.insert(std::make_pair("Forward", new misc::Forward(mouse)));
    m_algorithms.insert(std::make_pair("Wait", new misc::Wait(mouse)));
    m_algorithms.insert(std::make_pair("RightWallFollow", new wallFollow::RightWallFollow(mouse)));
    m_algorithms.insert(std::make_pair("LeftWallFollow", new wallFollow::LeftWallFollow(mouse)));
    m_algorithms.insert(std::make_pair("RandomizedWallFollow", new wallFollow::RandomizedWallFollow(mouse)));
}

const std::map<std::string, IAlgorithm*>& AlgoHub::getAlgorithms() const {
    return m_algorithms;
}
