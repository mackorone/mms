#ifndef LEFTWALLFOLLOW_H_
#define LEFTWALLFOLLOW_H_

#include "../IAlgorithm.h"

class LeftWallFollow : public IAlgorithm{

public:
    void solve(MouseInterface* mouse);

private:
    void leftWallFollowStep(MouseInterface* mouse);
};

#endif // LEFTWALLFOLLOW_H_
