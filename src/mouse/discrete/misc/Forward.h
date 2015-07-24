#pragma once

#include "../../IMouseAlgorithm.h"

namespace misc {

class Forward : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    sim::InterfaceType interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

};

} // namespace misc
