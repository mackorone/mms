#include "Algo.h"
#include <iostream>

void Algo::solve(Interface* interface) {

    interface->updateDeclareWallOnRead(true);
    interface->finalizeStaticOptions();

    int i = 0; 
    while (true) {
        std::cout << "move " << i++ << std::endl;
        if (!interface->wallLeft()){
            interface->turnLeft();
        }
        while (interface->wallFront()){
            interface->turnRight();
        }
        interface->moveForward();
    }
}
