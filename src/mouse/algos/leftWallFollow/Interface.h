#pragma once

#include <string>

class Interface {

public:

    int mazeWidth();
    int mazeHeight();

    bool wallFront();
    bool wallRight();
    bool wallLeft();

    void moveForward();
    void turnRight();
    void turnLeft();

    void setWall(int x, int y, char direction);
    void clearWall(int x, int y, char direction);

    void setColor(int x, int y, char color);
    void clearColor(int x, int y);
    void clearAllColor();

    void setText(int x, int y, std::string text);
    void clearText(int x, int y);
    void clearAllText();

    bool wasReset();
    void ackReset();

};
