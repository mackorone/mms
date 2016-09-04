#pragma once

#include "../IMouseAlgorithm.h"

namespace randomizedWallFollow {

class RandomizedWallFollow : public IMouseAlgorithm{

public:
    bool declareWallOnRead() const;
    bool declareBothWallHalves() const;
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, mms::MouseInterface* mouse);

private:
    void leftWallFollowStep(mms::MouseInterface* mouse);
    void rightWallFollowStep(mms::MouseInterface* mouse);

};

} // namespace randomizedWallFollow
