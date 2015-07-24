#pragma once

#include "IMazeAlgorithm.h"

class MazeAlgorithms {
    
public:
    MazeAlgorithms();
    ~MazeAlgorithms();
    const std::map<std::string, IMazeAlgorithm*>& getAlgorithms() const;

private:
    std::map<std::string, IMazeAlgorithm*> m_algorithms;

};
