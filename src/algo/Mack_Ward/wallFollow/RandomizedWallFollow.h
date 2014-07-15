#ifndef RANDOMIZEDWALLFOLLOW_H_
#define RANDOMIZEDWALLFOLLOW_H_

#include "../../IAlgorithm.h"

class RandomizedWallFollow : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
    void rightWallFollowStep(sim::MouseInterface* mouse);

};

#endif // RANDOMIZEDWALLFOLLOW_H_
