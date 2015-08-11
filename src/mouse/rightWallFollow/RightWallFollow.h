#pragma once

#include "../IMouseAlgorithm.h"

namespace rightWallFollow {

class RightWallFollow : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    std::string interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

private:
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

} // namespace rightWallFollow
