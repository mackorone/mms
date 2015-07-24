#include "MazeInterface.h"

#include "SimUtilities.h"

namespace sim {

MazeInterface::MazeInterface(std::vector<std::vector<BasicTile>>* basicMaze) : m_basicMaze(basicMaze) {
}

void MazeInterface::setWall(int x, int y, char direction, bool wallExists) {
    Direction d;
    switch (direction) {
        case 'n':
            d = NORTH;
            break;
        case 'e':
            d = EAST;
            break;
        case 's':
            d = SOUTH;
            break;
        case 'w':
            d = WEST;
            break;
        // TODO: MACK - default case
    }
    m_basicMaze->at(x).at(y).walls.at(d) = wallExists; // TODO: MACK - error checking
}

} // namespace sim
