#pragma once

#include "../sim/MazeInterface.h"

class IMazeAlgorithm {

public:
    virtual ~IMazeAlgorithm();

    virtual void generate(int mazeWidth, int mazeHeight, sim::MazeInterface* maze) = 0;

};
