#pragma once

#include "../IMouseAlgorithm.h"

namespace fontTest {

class FontTest : public IMouseAlgorithm {

public:
    std::string interfaceType() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

};

} // namespace fontTest
