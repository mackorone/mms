#include "Algo.h"

#include "unistd.h"

void Algo::solve(Interface* interface) {
    interface->moveForward();
    while (false) {
        if (!interface->wallLeft()){
            interface->turnLeft();
        }
        while (interface->wallFront()){
            interface->turnRight();
        }
        interface->moveForward();
    }
}
