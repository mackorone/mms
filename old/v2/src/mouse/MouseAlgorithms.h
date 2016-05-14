#pragma once

#include "IMouseAlgorithm.h"

class MouseAlgorithms {
    
public:
    // The MouseAlgorithms class is not constructible
    MouseAlgorithms() = delete;

    static bool isMouseAlgorithm(const std::string& str);
    static IMouseAlgorithm* getMouseAlgorithm(const std::string& str);

private:
    static std::pair<bool, IMouseAlgorithm*> helper(const std::string& str, bool justChecking);

};
