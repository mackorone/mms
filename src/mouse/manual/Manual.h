#pragma once

#include "../IMouseAlgorithm.h"

namespace manual {

class Manual : public IMouseAlgorithm {

public:
    std::string interfaceType() const;
    void solve(
        int mazeWidth, int mazeHeight, bool officialMaze,
        char initialDirection, sim::MouseInterface* mouse);

};

} // namespace misc
