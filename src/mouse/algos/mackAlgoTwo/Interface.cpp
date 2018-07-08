#include "Interface.h"

#include "Options.h"
#include "Printer.h"
#include "Reader.h"

#if (!SIMULATOR) 
extern bool walls_global[3];
extern volatile bool buttonPressed;
#endif

int Interface::getMazeWidth() {
#if (SIMULATOR)
    PRINT("getMazeWidth");
    READ_AND_RETURN_INT();
#else
    return 16;
#endif
}

int Interface::getMazeHeight() {
#if (SIMULATOR)
    PRINT("getMazeHeight");
    READ_AND_RETURN_INT();
#else
    return 16;
#endif
}

bool Interface::isWallFront() {
#if (SIMULATOR)
    PRINT("isWallFront");
    READ_AND_RETURN_BOOL();
#else
    return walls_global[1];
#endif
}

bool Interface::isWallRight() {
#if (SIMULATOR)
    PRINT("isWallRight");
    READ_AND_RETURN_BOOL();
#else
    return walls_global[2];
#endif
}

bool Interface::isWallLeft() {
#if (SIMULATOR)
    PRINT("isWallLeft");
    READ_AND_RETURN_BOOL();
#else
    return walls_global[0];
#endif
}

void Interface::moveForward() {
#if (SIMULATOR)
    PRINT("moveForward");
    READ();
#endif
}

void Interface::turnRight() {
#if (SIMULATOR)
    PRINT("turnRight");
    READ();
#endif
}

void Interface::turnLeft() {
#if (SIMULATOR)
    PRINT("turnLeft");
    READ();
#endif
}

void Interface::reset() {
#if (SIMULATOR)
    PRINT("reset");
    READ();
#endif
}

void Interface::setTileColor(int x, int y, char color) {
#if (SIMULATOR)
    PRINT("setTileColor", x, y, color);
#endif
}

void Interface::clearTileColor(int x, int y) {
#if (SIMULATOR)
    PRINT("clearTileColor", x, y);
#endif
}

void Interface::clearAllTileColor() {
#if (SIMULATOR)
    PRINT("clearAllTileColor");
#endif
}

void Interface::setTileText(int x, int y, const std::string& text) {
#if (SIMULATOR)
    PRINT("setTileText", x, y, text);
#endif
}

void Interface::clearTileText(int x, int y) {
#if (SIMULATOR)
    PRINT("clearTileText", x, y);
#endif
}

void Interface::clearAllTileText() {
#if (SIMULATOR)
    PRINT("clearAllTileText");
#endif
}

void Interface::declareWall(int x, int y, char direction, bool wallExists) {
#if (SIMULATOR)
    PRINT("declareWall", x, y, direction, boolToString(wallExists));
#endif
}

void Interface::undeclareWall(int x, int y, char direction) {
#if (SIMULATOR)
    PRINT("undeclareWall", x, y, direction);
#endif
}

bool Interface::wasInputButtonPressed(int inputButton) {
#if (SIMULATOR)
    PRINT("wasInputButtonPressed", inputButton);
    READ_AND_RETURN_BOOL();
#else
    if (inputButton == 2) {
        return buttonPressed;
    }
    return false;
#endif
}

void Interface::acknowledgeInputButtonPressed(int inputButton) {
#if (SIMULATOR)
    PRINT("acknowledgeInputButtonPressed", inputButton);
    READ();
#else
    if (inputButton == 2) {
        buttonPressed = false;
    }
#endif
}

std::string Interface::boolToString(bool value) {
    return value ? "true" : "false";
}
