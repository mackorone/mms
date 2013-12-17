#ifndef RIGHTWALLFOLLOW_H_
#define RIGHTWALLFOLLOW_H_

#include "IAlgorithm.h"

class RightWallFollow : public IAlgorithm{

public:
    void solve(MouseInterface* mouse);

private:
    void rightWallFollowStep(MouseInterface* mouse);

};

#endif // RIGHTWALLFOLLOW_H_
