#pragma once

#include "../IMouseAlgorithm.h"

namespace test {

class Test : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    std::string interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

};

} // namespace test
