#pragma once

#include <map>
#include <vector>

class Interface {

public:
    Interface(
        std::vector<std::vector<std::map<char, bool> > >* maze,
        bool* success);

    double getRandom();
    void setWall(int x, int y, char direction, bool wallExists);

private:
    std::vector<std::vector<std::map<char, bool> > >* m_maze;
    bool* m_success;

};
