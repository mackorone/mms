#pragma once

class MazeInterface {

public:
    MazeInterface();

    // Misc functions
    double getRandom();
    void quit();

    // Sets the wall
    void setWall(int x, int y, char direction, bool wallExists);

private:

};
