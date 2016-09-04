#include "Algo.h"
#include "Interface.h"
#include "Printer.h"

#include <cstdlib>
#include <iostream>

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

    // Initialize the algo and delimiter
    Algo algo;

    // Print the static mouse options
    PRINT("mouseFile", algo.mouseFile());
    PRINT("interfaceType", algo.interfaceType());
    PRINT("initialDirection", algo.initialDirection());
    PRINT("tileTextNumberOfRows", algo.tileTextNumberOfRows());
    PRINT("tileTextNumberOfCols", algo.tileTextNumberOfCols());
    PRINT("wheelSpeedFraction", algo.wheelSpeedFraction());

    // Call the solve method of the algo
    Interface interface;
    algo.solve(&interface);

    return 0;
}
