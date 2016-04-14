#pragma once

#include "../IMouseAlgorithm.h"

namespace diagonalTest {

class DiagonalTest : public IMouseAlgorithm {

public:
	bool useTileEdgeMovements() const;
	void justMoveForward(sim::MouseInterface* mouse);
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

};

} // namespace diagonalTest
