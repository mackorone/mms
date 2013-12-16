#ifndef START_H_
#define START_H_

#include "MouseInterface.h"

class Start{

public:
    Start(MouseInterface* mouse/*, Algorithm* algo*/);
    ~Start();
    void solve();

private:
    MouseInterface* m_mouse;
    //Algorithm* m_algo;

    // Built-in algorithms
    void rightWallFollowStep();
    void leftWallFollowStep();
    void rightWallFollow();
    void leftWallFollow();
    void randomizedWallFollow();

    // Floodfill
    void floodSolve();
    void print();
    void initializeGrid();
    void flood(int x, int y);
    void decide(int x, int y);
    void findLowest(int left, int up, int right, int down);
    void leftWall(int x, int y);
    void rightWall(int x, int y);
    void frontWall(int x, int y);
    void updateWall(int x, int y);
    void forward();
    void turnLeft();
    void turnRight();
    void turnAround();
};

#endif // START_H_
