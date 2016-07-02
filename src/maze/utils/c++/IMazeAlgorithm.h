#pragma once

#include "MazeInterface.h"

class IMazeAlgorithm {

public:
    virtual void generate(int mazeWidth, int mazeHeight, MazeInterface* maze) = 0;

};
