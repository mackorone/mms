#pragma once

#include <vector>

#include "BasicTile.h"

namespace sim {

class MazeInterface {

public:
    MazeInterface(std::vector<std::vector<BasicTile>>* basicMaze);

    // Logging functions
    void debug(const std::string& str);
    void info(const std::string& str);
    void warn(const std::string& str);
    void error(const std::string& str);

    // Misc functions
    double getRandom();
    void quit();

    // Sets the wall
    void setWall(int x, int y, char direction, bool wallExists);

private:
    std::vector<std::vector<BasicTile>>* m_basicMaze;
    int getWidth();
    int getHeight();

};

} // namespace sim
