#pragma once

#include "../../IMouseAlgorithm.h"

namespace wallFollow {

class LeftWallFollow : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    sim::InterfaceType interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

} // namespace wallFollow
