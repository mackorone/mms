#include "Algo.h"

#include "unistd.h"

void Algo::solve(Interface* interface) {

    interface->finalizeStaticOptions();

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
