#include "Algo.h"

void Algo::solve(Interface* interface) {

    interface->updateDeclareWallOnRead(true);
    interface->finalizeStaticOptions();

    interface->setMouseFile("megaMouse.xml");
    interface->setWheelSpeedFraction(0.1);
    interface->moveForward();
    interface->setWheelSpeedFraction(0.01);
    interface->moveForward();
    interface->setWheelSpeedFraction(0.1);
    interface->moveForward();
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
