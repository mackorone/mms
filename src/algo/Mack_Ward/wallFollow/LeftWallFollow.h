#ifndef LEFTWALLFOLLOW_H_
#define LEFTWALLFOLLOW_H_

#include "../../IAlgorithm.h"

class LeftWallFollow : public IAlgorithm{

public:
    void solve(sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

#endif // LEFTWALLFOLLOW_H_
