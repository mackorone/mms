#if(0)
//#ifndef FLOODFILL_H_
//#define FLOODFILL_H_

#include "IAlgorithm.h"

class FloodFill : public IAlgorithm{

public:
    void solve(MouseInterface* mouse);

private:
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
    
    MouseInterface* m_mouse;
};

#endif // FLOODFILL_H_
