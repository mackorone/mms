#include "diagonalTest.h"

namespace diagonalTest {

void DiagonalTest::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
	mouse->originMoveForwardToEdge();
	mouse->turnRightToEdge();
	mouse->turnRightToEdge();
	mouse->turnLeftToEdge();
	mouse->moveForwardToEdge(12);
	mouse->diagonalLeftLeft(3);
	mouse->moveForwardToEdge(6);
}

bool DiagonalTest::useTileEdgeMovements() const {
	// If false, the mouse will use basic movements
	// If true, it will use edge movements that allow for faster maze traversal
	return true;
}

void DiagonalTest::justMoveForward(sim::MouseInterface* mouse) {
	if (useTileEdgeMovements()) {
		// If we're using special tile edge movements, then the origin is a
		// special case, since we don't start on a tile edge.
		static bool firstMovement = true;
		if (firstMovement) {
			mouse->originMoveForwardToEdge();
			firstMovement = false;
		}
		else {
			mouse->moveForwardToEdge();
		}

	}
	else {
		mouse->moveForward();
	}
}

} // namespace diagonalTest
