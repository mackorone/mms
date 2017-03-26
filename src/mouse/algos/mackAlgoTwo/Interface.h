#pragma once

#include <string>

class Interface {

public:

    // ----- Functions for setting/updating mouse options ----- //

    // Static options (should set at the beginning)
    void useContinuousInterface();
    void setInitialDirection(char initialDirection);
    void setTileTextRowsAndCols(int numRows, int numCols);
    void setWheelSpeedFraction(double wheelSpeedFraction);

    // Dynamic options (can be updated any time)
    void updateAllowOmniscience(bool allowOmniscience);
    void updateAutomaticallyClearFog(bool automaticallyClearFog);
    void updateDeclareBothWallHalves(bool declareBothWallHalves);
    void updateSetTileTextWhenDistanceDeclared(
        bool setTileTextWhenDistanceDeclared);
    void updateSetTileBaseColorWhenDistanceDeclaredCorrectly(
        bool setTileBaseColorWhenDistanceDeclaredCorrectly);
    void updateDeclareWallOnRead(bool declareWallOnRead);
    void updateUseTileEdgeMovements(bool useTileEdgeMovements);

    // ----- Any interface methods ----- //

    // Starting information
    int mazeWidth();
    int mazeHeight();
    bool isOfficialMaze();
    char initialDirection();

    // Misc functions
    double getRandomFloat();
    int millis(); // # of milliseconds of sim time (adjusted based on sim speed) that have passed
    void delay(int milliseconds); // # of milliseconds of sim time (adjusted based on sim speed)
    void resetPosition(); // Reset position of the mouse

    // Input buttons
    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

    // ----- Tile appearance functions (these don't block) ----- //

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

    // Tile fog
    void setTileFogginess(int x, int y, bool foggy);

    // Tile distance, where a negative distance corresponds to inf distance
    void declareTileDistance(int x, int y, int distance);
    void undeclareTileDistance(int x, int y);

    // ----- Continuous interface methods ----- //

    // Get the magnitude of the max speed of any one wheel in rpm
    double getWheelMaxSpeed(const std::string& name);

    // Set the speed of any one wheel
    void setWheelSpeed(const std::string& name, double rpm);

    // Get the number of encoder ticks per revolution for a wheel
    double getWheelEncoderTicksPerRevolution(const std::string& name);

    // Read the encoder for a particular wheel
    int readWheelEncoder(const std::string& name);

    // Reset the encoder for a particular wheel to zero, but only if the encoder type is relative
    void resetWheelEncoder(const std::string& name);

    // Returns a value in [0.0, 1.0]
    double readSensor(const std::string& name);

    // Returns deg/s of rotation
    double readGyro();

    // ----- Any discrete interface methods ----- //

    bool wallFront();
    bool wallRight();
    bool wallLeft();

    // ----- Basic discrete interface methods ----- //

    void moveForward();
    void moveForward(int count);

    void turnLeft();
    void turnRight();

    void turnAroundLeft();
    void turnAroundRight();

    // ----- Special discrete interface methods ----- //

    void originMoveForwardToEdge();
    void originTurnLeftInPlace();
    void originTurnRightInPlace();

    void moveForwardToEdge();
    void moveForwardToEdge(int count);

    void turnLeftToEdge();
    void turnRightToEdge();

    void turnAroundLeftToEdge();
    void turnAroundRightToEdge();

    void diagonalLeftLeft(int count);
    void diagonalLeftRight(int count);
    void diagonalRightLeft(int count);
    void diagonalRightRight(int count);

    // ----- Omniscience methods ----- //

    int currentXTile();
    int currentYTile();
    char currentDirection();

    double currentXPosMeters();
    double currentYPosMeters();
    double currentRotationDegrees();

private:

    std::string boolToString(bool value);

};
