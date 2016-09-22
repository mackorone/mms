#include "Algo.h"

std::string Algo::mouseFile() const {
    return "default.xml";
}

std::string Algo::interfaceType() const {
    return "DISCRETE";
}

std::string Algo::initialDirection() const {
    return "NORTH";
}

int Algo::tileTextNumberOfRows() const {
    return 2;
}

int Algo::tileTextNumberOfCols() const {
    return 3;
}

bool Algo::allowOmniscience() const {
    return false;
}

bool Algo::automaticallyClearFog() const {
    return true;
}

bool Algo::declareBothWallHalves() const {
    return true;
}

bool Algo::setTileTextWhenDistanceDeclared() const {
    return true;
}

bool Algo::setTileBaseColorWhenDistanceDeclaredCorrectly() const {
    return false;
}

double Algo::wheelSpeedFraction() const {
    return 1.0; 
}

bool Algo::declareWallOnRead() const {
    return true;
}

bool Algo::useTileEdgeMovements() const {
    return false;
}

void Algo::solve(Interface* interface) {
    // TODO: MACK
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
