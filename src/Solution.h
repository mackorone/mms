#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "MouseInterface.h"

class Solution{

public:
    Solution(MouseInterface* mouse);
    ~Solution();
    void solve();

private:
    MouseInterface* m_mouse;

    // Built-in algorithms
    void rightWallStep();
    void leftWallStep();
    void rightWallFollow();
    void leftWallFollow();
    void randomizedWallFollow();
};

#endif // SOLUTION_H_
