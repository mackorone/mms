#include "MazeAlgorithms.h"

#include <map>

#include "randomize/Randomize.h"

MazeAlgorithms::MazeAlgorithms() {
    m_algorithms.insert(std::make_pair("Randomize", new randomize::Randomize()));
}

MazeAlgorithms::~MazeAlgorithms() {
    for (std::pair<std::string, IMazeAlgorithm*> pair : m_algorithms) {
        delete pair.second;
    }
}

const std::map<std::string, IMazeAlgorithm*>& MazeAlgorithms::getAlgorithms() const {
    return m_algorithms;
}
