#include "MouseAlgorithms.h"

#include <map>

#include "manual/Manual.h"
#include "continuous/Continuous.h"
#include "doNothing/DoNothing.h"
#include "forward/Forward.h"
#include "leftWallFollow/LeftWallFollow.h"
#include "rightWallFollow/RightWallFollow.h"
#include "randomizedWallFollow/RandomizedWallFollow.h"
#include "floodFill/FloodFill.h"
#include "mackAlgo/MackAlgo.h"

MouseAlgorithms::MouseAlgorithms() {
    m_algorithms.insert(std::make_pair("Manual", new manual::Manual()));
    m_algorithms.insert(std::make_pair("Continuous", new continuous::Continuous()));
    m_algorithms.insert(std::make_pair("DoNothing", new doNothing::DoNothing()));
    m_algorithms.insert(std::make_pair("Forward", new forward::Forward()));
    m_algorithms.insert(std::make_pair("LeftWallFollow", new leftWallFollow::LeftWallFollow()));
    m_algorithms.insert(std::make_pair("RightWallFollow", new rightWallFollow::RightWallFollow()));
    m_algorithms.insert(std::make_pair("RandomizedWallFollow", new randomizedWallFollow::RandomizedWallFollow()));
    m_algorithms.insert(std::make_pair("FloodFill", new floodFill::FloodFill()));
    m_algorithms.insert(std::make_pair("MackAlgo", new mackAlgo::MackAlgo()));
}

MouseAlgorithms::~MouseAlgorithms() {
    for (std::pair<std::string, IMouseAlgorithm*> pair : m_algorithms) {
        delete pair.second;
    }
}

const std::map<std::string, IMouseAlgorithm*>& MouseAlgorithms::getAlgorithms() const {
    return m_algorithms;
}
