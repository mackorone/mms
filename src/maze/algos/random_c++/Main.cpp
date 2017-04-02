#include "Algo.h"
#include "Interface.h"
#include "Printer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

int main(int argc, char* argv[]) {

    // Print the usage
    if (argc < 3 || 4 < argc) {
        std::cout << "Usage: a.out <WIDTH> <HEIGHT> [<SEED>]"
                  << std::endl;
        return 1;
    }

    // Read the width and height args
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    if (width <= 0 || height <= 0) {
        std::cout << "Error: <WIDTH> and <HEIGHT> must be positive integers"
                  << std::endl;
        return 1;
    }

    // Read the seed arg
    int seed = time(NULL);
    if (argc == 4) {
        seed = atoi(argv[3]);
        if (seed < 0) {
            std::cout << "Error: <SEED> must be a non-negative integer"
                      << std::endl;
            return 1;
        }
    }

    // Seed rand()
    srand(seed);

    // Generate an empty maze
    std::vector<std::vector<std::map<char, bool> > > maze;
    for (int i = 0; i < width; i += 1) {
        std::vector<std::map<char, bool> > column;
        for (int j = 0; j < height; j += 1) {
            std::map<char, bool> cell;
            cell['n'] = false;
            cell['e'] = false;
            cell['s'] = false;
            cell['w'] = false;
            column.push_back(cell);
        }
        maze.push_back(column);
    }

    bool success = true;

    Interface interface(&maze, &success);
    Algo().generate(&interface);

    if (!success) {
        return 1;
    }

    Printer::print(&maze);
    return 0;
}
