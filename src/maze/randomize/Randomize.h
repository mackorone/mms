#pragma once

#include "../IMazeAlgorithm.h"

namespace randomize {

class Randomize : public IMazeAlgorithm {

public:
    void generate(int mazeWidth, int mazeHeight, sim::MazeInterface* maze);

};

} // namespace randomize
