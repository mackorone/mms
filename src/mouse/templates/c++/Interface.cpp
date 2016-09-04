#include "Interface.h"

#include "Printer.h"

// TODO: MACK - wait for response

int Interface::mazeWidth() {
    PRINT("mazeWidth");
}

int Interface::mazeHeight() {
    PRINT("mazeHeight");
}

bool Interface::isOfficialMaze() {
    PRINT("isOfficialMaze");
}

char Interface::initialDirection() {
    PRINT("initialDirection");
}

double Interface::getRandomFloat() {
    PRINT("getRandomFloat");
}

int Interface::millis() {
    PRINT("millis");
}

void Interface::delay(int milliseconds) {
    PRINT("delay", milliseconds);
}

void Interface::setTileColor(int x, int y, char color) {
    PRINT("setTileColor", x, y, color);
}

void Interface::clearTileColor(int x, int y) {
    PRINT("clearTileColor", x, y);
}

void Interface::clearAllTileColor() {
    PRINT("clearAllTileColor");
}

void Interface::setTileText(int x, int y, const std::string& text) {
    PRINT("setTileText", x, y, text);
}

void Interface::clearTileText(int x, int y) {
    PRINT("clearTileText", x, y);
}

void Interface::clearAllTileText() {
    PRINT("clearTileText");
}

void Interface::declareWall(int x, int y, char direction, bool wallExists) {
    PRINT("declareWall", x, y, direction, wallExists);
}

void Interface::undeclareWall(int x, int y, char direction) {
    PRINT("undeclareWall", x, y, direction);
}

void Interface::setTileFogginess(int x, int y, bool foggy) {
    PRINT("setTileFogginess", x, y, foggy);
}

void Interface::declareTileDistance(int x, int y, int distance) {
    PRINT("declareTileDistance", x, y, distance);
}

void Interface::undeclareTileDistance(int x, int y) {
    PRINT("undeclareTileDistance", x, y);
}

void Interface::resetPosition() {
    PRINT("resetPosition");
}

bool Interface::inputButtonPressed(int inputButton) {
    PRINT("inputButtonPressed", inputButton);
}

void Interface::acknowledgeInputButtonPressed(int inputButton) {
    PRINT("acknowledgeInputButtonPressed", inputButton);
}

double Interface::getWheelMaxSpeed(const std::string& name) {
    PRINT("getWheelMaxSpeed", name);
}

void Interface::setWheelSpeed(const std::string& name, double rpm) {
    PRINT("setWheelSpeed", name, rpm);
}

double Interface::getWheelEncoderTicksPerRevolution(const std::string& name) {
    PRINT("getWheelEncoderTicksPerRevolution", name);
}

int Interface::readWheelEncoder(const std::string& name) {
    PRINT("readWheelEncoder", name);
}

void Interface::resetWheelEncoder(const std::string& name) {
    PRINT("resetWheelEncoder", name);
}

double Interface::readSensor(const std::string& name) {
    PRINT("readSensor", name);
}

double Interface::readGyro() {
    PRINT("readGyro");
}

bool Interface::wallFront() {
    PRINT("wallFront");
}

bool Interface::wallRight() {
    PRINT("wallRight");
}

bool Interface::wallLeft() {
    PRINT("wallLeft");
}

void Interface::moveForward() {
    PRINT("moveForward");
}

void Interface::moveForward(int count) {
    PRINT("moveForward", count);
}

void Interface::turnLeft() {
    PRINT("turnLeft");
}

void Interface::turnRight() {
    PRINT("turnRight");
}

void Interface::turnAroundLeft() {
    PRINT("turnAroundLeft");
}

void Interface::turnAroundRight() {
    PRINT("turnAroundRight");
}

void Interface::originMoveForwardToEdge() {
    PRINT("originMoveForwardToEdge");
}

void Interface::originTurnLeftInPlace() {
    PRINT("originTurnLeftInPlace");
}

void Interface::originTurnRightInPlace() {
    PRINT("originTurnRightInPlace");
}

void Interface::moveForwardToEdge() {
    PRINT("moveForwardToEdge");
}

void Interface::moveForwardToEdge(int count) {
    PRINT("moveForwardToEdge", count);
}

void Interface::turnLeftToEdge() {
    PRINT("turnLeftToEdge");
}

void Interface::turnRightToEdge() {
    PRINT("turnRightToEdge");
}

void Interface::turnAroundLeftToEdge() {
    PRINT("turnAroundLeftToEdge");
}

void Interface::turnAroundRightToEdge() {
    PRINT("turnAroundRightToEdge");
}

void Interface::diagonalLeftLeft(int count) {
    PRINT("diagonalLeftLeft", count);
}

void Interface::diagonalLeftRight(int count) {
    PRINT("diagonalLeftRight", count);
}

void Interface::diagonalRightLeft(int count) {
    PRINT("diagonalRightLeft", count);
}

void Interface::diagonalRightRight(int count) {
    PRINT("diagonalRightRight", count);
}

int Interface::currentXTile() {
    PRINT("currentXTile");
}

int Interface::currentYTile() {
    PRINT("currentYTile");
}

char Interface::currentDirection() {
    PRINT("currentDirection");
}

double Interface::currentXPosMeters() {
    PRINT("currentXPosMeters");
}

double Interface::currentYPosMeters() {
    PRINT("currentYPosMeters");
}

double Interface::currentRotationDegrees() {
    PRINT("currentRotationDegrees");
}
