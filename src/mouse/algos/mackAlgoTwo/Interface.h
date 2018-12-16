#pragma once

#include <string>

class Interface {

public:

    // Maze Dimensions
    int getMazeWidth();
    int getMazeHeight();

    // Wall existence
    bool isWallFront();
    bool isWallRight();
    bool isWallLeft();

    // Movement
    void moveForward();
    void turnRight();
    void turnLeft();

    // Tile color
    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    // Tile text
    void setTileText(int x, int y, const std::string& text);
    void clearTileText(int x, int y);
    void clearAllTileText();

    // Tile walls
    void declareWall(int x, int y, char direction, bool wallExists);
    void undeclareWall(int x, int y, char direction);

    // Input buttons
    bool wasReset();
    void ackReset();

private:

    std::string boolToString(bool value);

};
