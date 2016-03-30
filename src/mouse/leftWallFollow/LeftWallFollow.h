#pragma once

#include "../IMouseAlgorithm.h"

namespace leftWallFollow {

class LeftWallFollow : public IMouseAlgorithm {

public:
    bool automaticallyClearFog() const;
    bool declareWallOnRead() const;
    bool declareBothWallHalves() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    sim::MouseInterface* m_mouse;
    void leftWallFollowStep();
};

} // namespace leftWallFollow
