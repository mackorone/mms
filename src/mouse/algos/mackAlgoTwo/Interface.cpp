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
    PRINT("getWidth");
    READ_AND_RETURN_INT();
#else
    return 16;
#endif
}

int Interface::getMazeHeight() {
#if (SIMULATOR)
    PRINT("getHeight");
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

void Interface::setTileColor(int x, int y, char color) {
#if (SIMULATOR)
    PRINT("setColor", x, y, color);
#endif
}

void Interface::clearTileColor(int x, int y) {
#if (SIMULATOR)
    PRINT("clearColor", x, y);
#endif
}

void Interface::clearAllTileColor() {
#if (SIMULATOR)
    PRINT("clearAllColor");
#endif
}

void Interface::setTileText(int x, int y, const std::string& text) {
#if (SIMULATOR)
    PRINT("setText", x, y, text);
#endif
}

void Interface::clearTileText(int x, int y) {
#if (SIMULATOR)
    PRINT("clearText", x, y);
#endif
}

void Interface::clearAllTileText() {
#if (SIMULATOR)
    PRINT("clearAllText");
#endif
}

void Interface::declareWall(int x, int y, char direction, bool wallExists) {
#if (SIMULATOR)
    PRINT("setWall", x, y, direction, boolToString(wallExists));
#endif
}

void Interface::undeclareWall(int x, int y, char direction) {
#if (SIMULATOR)
    PRINT("clearWall", x, y, direction);
#endif
}

bool Interface::wasReset() {
#if (SIMULATOR)
    PRINT("wasReset");
    READ_AND_RETURN_BOOL();
#else
    if (inputButton == 2) {
        return buttonPressed;
    }
    return false;
#endif
}

void Interface::ackReset() {
#if (SIMULATOR)
    PRINT("ackReset");
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
