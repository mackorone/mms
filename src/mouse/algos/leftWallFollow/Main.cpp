#include <cstdlib>
#include <iostream>

// TODO: MACk
#include <unistd.h>

#include "Algo.h"
#include "Interface.h"
#include "Printer.h"

int main(int argc, char* argv[]) {

    // Print the usage
    if (2 < argc) {
        std::cout << "Usage: a.out [<SEED>]" << std::endl;
        return 1;
    }

    // Read the seed arg
    int seed = time(NULL);
    if (argc == 2) {
        seed = atoi(argv[1]);
        if (seed <= 0) {
            std::cout << "Error: <SEED> must be a positive integer"
                      << std::endl;
            return 1;
        }
    }

    // Seed rand()
    srand(seed);

    // Initialize the algo
    Algo algo;

    // TODO: They're all static for now
    // Print the static mouse options
    PRINT("mouseFile", algo.mouseFile());
    sleep(1);
    PRINT("interfaceType", algo.interfaceType());
    sleep(1);
    PRINT("initialDirection", algo.initialDirection());
    sleep(1);
    PRINT("tileTextNumberOfRows", algo.tileTextNumberOfRows());
    sleep(1);
    PRINT("tileTextNumberOfCols", algo.tileTextNumberOfCols());
    sleep(1);
    PRINT("wheelSpeedFraction", algo.wheelSpeedFraction());
    sleep(1);
    PRINT("allowOmniscience", algo.allowOmniscience());
    sleep(1);
    PRINT("automaticallyClearFog", algo.automaticallyClearFog());
    sleep(1);
    PRINT("declareBothWallHalves", algo.declareBothWallHalves());
    sleep(1);
    PRINT("setTileTextWhenDistanceDeclared",
        algo.setTileTextWhenDistanceDeclared());
    sleep(1);
    PRINT("setTileBaseColorWhenDistanceDeclaredCorrectly",
        algo.setTileBaseColorWhenDistanceDeclaredCorrectly());
    sleep(1);
    PRINT("declareWallOnRead", algo.declareWallOnRead());
    sleep(1);
    PRINT("useTileEdgeMovements", algo.useTileEdgeMovements());

    // TODO: MACK - wait to receive the OK here

    // Call the solve method of the algo
    Interface interface;
    algo.solve(&interface);

    return 0;
}
