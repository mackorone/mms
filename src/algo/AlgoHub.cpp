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

AlgoHub::AlgoHub() {
    m_algorithms.insert(std::make_pair("Continue", new demo::Continue()));
    m_algorithms.insert(std::make_pair("FloodFill", new floodfill::FloodFill()));
    m_algorithms.insert(std::make_pair("MackAlgo", new mack::MackAlgo()));
    m_algorithms.insert(std::make_pair("Forward", new misc::Forward()));
    m_algorithms.insert(std::make_pair("Wait", new misc::Wait()));
    m_algorithms.insert(std::make_pair("RightWallFollow", new wallFollow::RightWallFollow()));
    m_algorithms.insert(std::make_pair("LeftWallFollow", new wallFollow::LeftWallFollow()));
    m_algorithms.insert(std::make_pair("RandomizedWallFollow", new wallFollow::RandomizedWallFollow()));
}

const std::map<std::string, IAlgorithm*>& AlgoHub::getAlgorithms() const {
    return m_algorithms;
}
