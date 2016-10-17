#include <cstdlib>
#include <iostream>

#include "Algo.h"
#include "Interface.h"

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
    // PRINT("mouseFile", algo.mouseFile());
    // PRINT("interfaceType", algo.interfaceType());
    // PRINT("initialDirection", algo.initialDirection());
    // PRINT("tileTextNumberOfRows", algo.tileTextNumberOfRows());
    // PRINT("tileTextNumberOfCols", algo.tileTextNumberOfCols());
    // PRINT("wheelSpeedFraction", algo.wheelSpeedFraction());
    // PRINT("allowOmniscience", algo.allowOmniscience());
    // PRINT("automaticallyClearFog", algo.automaticallyClearFog());
    // PRINT("declareBothWallHalves", algo.declareBothWallHalves());
    // PRINT("setTileTextWhenDistanceDeclared",
    //     algo.setTileTextWhenDistanceDeclared());
    // PRINT("setTileBaseColorWhenDistanceDeclaredCorrectly",
    //     algo.setTileBaseColorWhenDistanceDeclaredCorrectly());
    // PRINT("declareWallOnRead", algo.declareWallOnRead());
    // PRINT("useTileEdgeMovements", algo.useTileEdgeMovements());

    // TODO: MACK - wait to receive the OK here
    // TODO: MACK - add API for finalizing static options, adverstising all options

    // Call the solve method of the algo
    Interface interface;
    algo.solve(&interface);

    return 0;
}
