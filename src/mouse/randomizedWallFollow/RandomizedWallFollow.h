#pragma once

#include "../IMouseAlgorithm.h"

namespace randomizedWallFollow {

class RandomizedWallFollow : public IMouseAlgorithm{

public:
    std::string mouseFile() const;
    sim::InterfaceType interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace randomizedWallFollow
