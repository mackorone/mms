#pragma once

#include "IAlgorithm.h"

class AlgoHub {
    
public:
    AlgoHub();
    const std::map<std::string, IAlgorithm*>& getAlgorithms() const;

private:
    std::map<std::string, IAlgorithm*> m_algorithms;

};
