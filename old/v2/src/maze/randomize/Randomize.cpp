#include "Randomize.h"

namespace randomize {

void Randomize::generate(int mazeWidth, int mazeHeight, sim::MazeInterface* maze) {

    // Percentage chance any one wall will exist
    double wallProb = 0.40;
    for (int x = 0; x < mazeWidth; x += 1) {
        for (int y = 0; y < mazeHeight; y += 1) {
            for (char c : {'n', 'e', 's', 'w'}) {
                bool wallExists = maze->getRandom() <= wallProb;
                switch (c) {
                    case 'n':
                        if (y + 1 < mazeHeight) {
                            maze->setWall(x, y, 'n', wallExists);
                            maze->setWall(x, y + 1, 's', wallExists);
                        }
                        else {
                            maze->setWall(x, y, 'n', true);
                        }
                        break;
                    case 'e':
                        if (x + 1 < mazeWidth) {
                            maze->setWall(x, y, 'e', wallExists);
                            maze->setWall(x + 1, y, 'w', wallExists);
                        }
                        else {
                            maze->setWall(x, y, 'e', true);
                        }
                        break;
                    case 's':
                        if (y > 0) {
                            maze->setWall(x, y, 's', wallExists);
                            maze->setWall(x, y - 1, 'n', wallExists);
                        }
                        else {
                            maze->setWall(x, y, 's', true);
                        }
                        break;
                    case 'w':
                        if (x > 0) {
                            maze->setWall(x, y, 'w', wallExists);
                            maze->setWall(x - 1, y, 'e', wallExists);
                        }
                        else {
                            maze->setWall(x, y, 'w', true);
                        }
                        break;
                }
            }
        }
    }
}

} // namespace randomize
