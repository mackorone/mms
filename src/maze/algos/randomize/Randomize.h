#pragma once

#include "../IMazeAlgorithm.h"

namespace randomize {

class Randomize : public IMazeAlgorithm {

public:
    void generate(int mazeWidth, int mazeHeight, MazeInterface* maze);

};

} // namespace randomize
