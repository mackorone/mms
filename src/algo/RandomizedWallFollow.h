#ifndef RANDOMIZEDWALLFOLLOW_H_
#define RANDOMIZEDWALLFOLLOW_H_

#include "IAlgorithm.h"

class RandomizedWallFollow : public IAlgorithm{

public:
    void solve(MouseInterface* mouse);

private:
    void leftWallFollowStep(MouseInterface* mouse);
    void rightWallFollowStep(MouseInterface* mouse);

};

#endif // RANDOMIZEDWALLFOLLOW_H_
