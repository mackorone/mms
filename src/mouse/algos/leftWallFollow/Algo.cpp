#include "Algo.h"

void Algo::solve(Interface* interface) {

    interface->updateDeclareWallOnRead(true);
    interface->finalizeStaticOptions();

    interface->moveForward();
    interface->setTileText(0, 0, "abcdef");
    interface->delay(500);
    interface->setTileTextRowsAndCols(1, 6);
    interface->delay(500);
    interface->setTileText(0, 0, "123456");
    interface->delay(500);
    interface->setTileTextRowsAndCols(2, 3);
    interface->delay(500);
    interface->setTileText(0, 0, "abcdef");
    interface->delay(500);

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
