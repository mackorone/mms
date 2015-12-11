#pragma once

#include "InterfaceType.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "Options.h"
#include "Param.h"

// Windows compatibility
#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#define ENSURE_DISCRETE_INTERFACE ensureDiscreteInterface(__func__);
#define ENSURE_CONTINUOUS_INTERFACE ensureContinuousInterface(__func__);

namespace manual {
    class Manual;
}

namespace sim {

class MouseInterface {

public:
    MouseInterface(const Maze* maze, Mouse* mouse, MazeGraphic* mazeGraphic);

    // ----- Any interface methods ----- //

    // Logging functions
    void debug(const std::string& str);
    void info(const std::string& str);
    void warn(const std::string& str);
    void error(const std::string& str);

    // Misc functions
    double getRandom();
    void delay(int milliseconds);
    void quit();

    // Tile color
    void setTileColor(int x, int y, char color);
    void clearTileColor(int x, int y);
    void clearAllTileColor();

    // Tile walls
    void declareWall(int x, int y, char direction, bool wallExists);
    void undeclareWall(int x, int y, char direction);

    // Tile fog
    void setTileFogginess(int x, int y, bool foggy);

    // Tile distance
    // NOTE: a negative distance corresponds to inf distance
    void declareTileDistance(int x, int y, int distance);
    void undeclareTileDistance(int x, int y);

    // Reset position of the mouse
    void resetPosition();

    // Input buttons
    bool inputButtonPressed(int inputButton);
    void acknowledgeInputButtonPressed(int inputButton);

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
    double readSensor(std::string name);

    // Returns deg/s of rotation
    double readGyro();

    // ----- Discrete interface methods ----- //

    bool wallFront();
    bool wallRight();
    bool wallLeft();
    void moveForward();
    void turnLeft();
    void turnRight();
    void turnAround();

private:
    const Maze* m_maze;
    Mouse* m_mouse;
    MazeGraphic* m_mazeGraphic;

    // TODO: MACK - genericize this
    std::set<std::pair<int, int>> m_tilesWithColor;

    // Algo options
    friend class Driver; // TODO: MACK
    void setOptions(const Options& options);
    Options m_options;

    void ensureDiscreteInterface(const std::string& callingFunction) const;
    void ensureContinuousInterface(const std::string& callingFunction) const;

    bool isWall(std::pair<int, int> position, Direction direction);
    bool hasOpposingWall(int x, int y, Direction direction) const;
    std::pair<std::pair<int, int>, Direction> getOpposingWall(int x, int y, Direction direction) const;

    friend class manual::Manual; // TODO: MACK
    std::pair<int, int> getCurrentDiscretizedTranslation() const;
    Direction getCurrentDiscretizedRotation() const;
};

} // namespace sim
