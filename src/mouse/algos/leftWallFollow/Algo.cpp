#include "Algo.h"

void Algo::solve(Interface* interface) {

    interface->updateDeclareWallOnRead(true);
    interface->finalizeStaticOptions();

    interface->moveForward();
    interface->setMouseFile("omniMouse.xml");
    interface->moveForward();
    interface->setMouseFile("default.xml");
    interface->turnRight();
    interface->moveForward();
    interface->setMouseFile("default.xml");
    /*
    while (true) {
        if (!interface->wallLeft()){
            interface->turnLeft();
        }
        while (interface->wallFront()){
            interface->turnRight();
        }
        interface->moveForward();
    }
    */
}
