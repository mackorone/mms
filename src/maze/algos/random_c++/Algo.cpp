#include "Algo.h"

#include <string>

void Algo::generate(Interface* interface) {
    std::string directions = "nesw";
    for (int x = 0; x < interface->getWidth(); x += 1) {
        for (int y = 0; y < interface->getHeight(); y += 1) {
            for (int i = 0; i < directions.size(); i += 1) {
                bool wallExists = interface->getRandomFloat() <= 0.40;
                interface->setWall(x, y, directions.at(i), wallExists);
            }
        }
    }
}
