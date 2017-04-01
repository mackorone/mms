#include "Algo.h"

void Algo::solve(Interface* interface) {

    interface->updateDeclareWallOnRead(true);
    interface->delay(3000);

    while (true) {
        if (!interface->wallLeft()){
            interface->turnLeft();
        }
        while (interface->wallFront()){
            interface->turnRight();
        }
        interface->moveForward();
    }
}
