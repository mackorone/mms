#pragma once

#include "IMouseAlgorithm.h"

class MouseAlgorithms {
    
public:
    MouseAlgorithms();
    ~MouseAlgorithms();
    const std::map<std::string, IMouseAlgorithm*>& getAlgorithms() const;

private:
    std::map<std::string, IMouseAlgorithm*> m_algorithms;

};
