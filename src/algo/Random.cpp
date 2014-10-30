#include "Random.h"


void Random::solve(sim::MouseInterface* mouse) {
while (true) {
if (!mouse->wallFront() && !(rand()%20==0)){
	mouse->moveForward();
	}
else if (!mouse->wallRight() && rand()%2==0){
	mouse->turnRight();
	mouse->moveForward();
	}
else if (!mouse->wallLeft()){
	mouse->turnLeft();
	mouse->moveForward();
	}
else {
	mouse->turnRight();
	mouse->turnRight();
	while (mouse->wallRight() && mouse->wallLeft()) {
		mouse->moveForward();
	}
	if (!mouse->wallRight() && rand()%2==0) {
		mouse->turnRight();
		mouse->moveForward();
		}
	else if (!mouse->wallLeft()) {
		mouse->turnLeft();
		mouse->moveForward();
	}
}
}
    // ---- REMINDER ---- //
    /* Valid function calls:
     * 1) mouse->wallFront()
     * 2) mouse->wallRight()
     * 3) mouse->wallLeft()
     * 4) mouse->moveForward()
     * 5) mouse->turnRight()
     * 6) mouse->turnLeft()
     */
}
