#include "Interface.h"

#include <cstdlib>
#include <iostream>

Interface::Interface(
    std::vector<std::vector<std::map<char, bool> > >* maze,
    bool* success) :
    m_maze(maze),
    m_success(success) {
}

double Interface::getRandom() {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

void Interface::setWall(int x, int y, char direction, bool wallExists) {

    // Sanity checks
    if (x < 0 || m_maze->size() <= x) {
        std::cout << "Error: x value " << x << " is invalid"
                  << std::endl;
        *m_success = false;
        return;
    }
    if (y < 0 || m_maze->size() <= y) {
        std::cout << "Error: y value " << y << " is invalid"
                  << std::endl;
        *m_success = false;
        return;
    }
    if (direction != 'n' && direction != 'e' && direction != 's' && direction != 'w') {
        std::cout << "Error: direction value '" << direction << "' is invalid"
                  << std::endl;
        *m_success = false;
        return;
    }

    // Set the wall value
    m_maze->at(x).at(y).at(direction) = wallExists;

    // Set the opposing wall value
    switch (direction) {
        case 'n': {
            if (y < m_maze->at(x).size()) {
                m_maze->at(x).at(y + 1).at('s') = wallExists;
            }
            break;
        }
        case 'e': {
            if (x < m_maze->size()) {
                m_maze->at(x + 1).at(y).at('w') = wallExists;
            }
            break;
        }
        case 's': {
            if (0 < y) {
                m_maze->at(x).at(y - 1).at('n') = wallExists;
            }
            break;
        }
        case 'w': {
            if (0 < x) {
                m_maze->at(x - 1).at(y).at('e') = wallExists;
            }
            break;
        }
    }
}
