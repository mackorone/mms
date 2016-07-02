#include "MazeInterface.h"

MazeInterface::MazeInterface() {
}

void MazeInterface::quit() {
}

double MazeInterface::getRandom() {
    return 0.5;
}

void MazeInterface::setWall(int x, int y, char direction, bool wallExists) {

    /*
    if (x < 0 || getWidth() <= x || y < 0 || getHeight() <= y) {
        L()->warn(
            "The generated maze width and height values are %v and %v, respectively."
            " There is no tile at position (%v, %v), and thus you cannot set its wall value.",
            getWidth(), getHeight(), x, y);
        return;
    }

    if (!ContainerUtilities::mapContains(CHAR_TO_DIRECTION, direction)) {
        L()->warn("The character '%v' is not mapped to a valid direction.", direction);
        return;
    }

    m_basicMaze->at(x).at(y).walls.at(CHAR_TO_DIRECTION.at(direction)) = wallExists;
    */
}
