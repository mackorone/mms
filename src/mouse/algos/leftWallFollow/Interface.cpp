#include "Interface.h"

#include "Printer.h"
#include "Reader.h"

int Interface::mazeWidth() {
    PRINT("mazeWidth");
    READ_AND_RETURN_INT();
}

int Interface::mazeHeight() {
    PRINT("mazeHeight");
    READ_AND_RETURN_INT();
}

bool Interface::wallFront() {
    PRINT("wallFront");
    READ_AND_RETURN_BOOL();
}

bool Interface::wallRight() {
    PRINT("wallRight");
    READ_AND_RETURN_BOOL();
}

bool Interface::wallLeft() {
    PRINT("wallLeft");
    READ_AND_RETURN_BOOL();
}

void Interface::moveForward() {
    PRINT("moveForward");
    READ();
}

void Interface::turnRight() {
    PRINT("turnRight");
    READ();
}

void Interface::turnLeft() {
    PRINT("turnLeft");
    READ();
}

void Interface::setWall(int x, int y, char direction) {
    PRINT("setWall", x, y, direction);
}

void Interface::clearWall(int x, int y, char direction) {
    PRINT("clearWall", x, y, direction);
}

void Interface::setColor(int x, int y, char color) {
    PRINT("setColor", x, y, color);
}

void Interface::clearColor(int x, int y) {
    PRINT("clearColor", x, y);
}

void Interface::clearAllColor() {
    PRINT("clearAllColor");
}

void Interface::setText(int x, int y, std::string text) {
    PRINT("setText", x, y, text);
}

void Interface::clearText(int x, int y) {
    PRINT("clearText", x, y);
}

void Interface::clearAllText() {
    PRINT("clearAllText");
}

bool Interface::wasReset() {
    PRINT("wasReset");
    READ_AND_RETURN_BOOL();
}

void Interface::ackReset() {
    PRINT("ackReset");
    READ();
}
