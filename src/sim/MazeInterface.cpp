#include "MazeInterface.h"

#include "ContainerUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

MazeInterface::MazeInterface(std::vector<std::vector<BasicTile>>* basicMaze) : m_basicMaze(basicMaze) {
}

void MazeInterface::debug(const std::string& str) {
    Logging::getMazeLogger()->debug(str);
}

void MazeInterface::info(const std::string& str) {
    Logging::getMazeLogger()->info(str);
}

void MazeInterface::warn(const std::string& str) {
    Logging::getMazeLogger()->warn(str);
}

void MazeInterface::error(const std::string& str) {
    Logging::getMazeLogger()->error(str);
}

void MazeInterface::quit() {
    SimUtilities::quit();
}

double MazeInterface::getRandom() {
    return SimUtilities::getRandom();
}

void MazeInterface::setWall(int x, int y, char direction, bool wallExists) {

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
}

int MazeInterface::getWidth() {
    return m_basicMaze->size();
}

int MazeInterface::getHeight() {
    return (m_basicMaze->size() > 0 ? m_basicMaze->at(0).size() : 0);
}

} // namespace sim
