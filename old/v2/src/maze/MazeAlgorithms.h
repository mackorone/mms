#pragma once

#include "IMazeAlgorithm.h"

class MazeAlgorithms {
    
public:
    // The MazeAlgorithms class is not constructible
    MazeAlgorithms() = delete;

    static bool isMazeAlgorithm(const std::string& str);
    static IMazeAlgorithm* getMazeAlgorithm(const std::string& str);

private:
    static std::pair<bool, IMazeAlgorithm*> helper(const std::string& str, bool justChecking);

};
