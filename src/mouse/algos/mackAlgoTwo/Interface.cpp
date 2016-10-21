#include "Interface.h"

#include "Printer.h"
#include "Reader.h"

void Interface::setMouseFile(const std::string& mouseFile) {
    PRINT("setMouseFile", mouseFile);
    READ();
}

void Interface::setInterfaceType(const std::string& interfaceType) {
    PRINT("setInterfaceType", interfaceType);
    READ();
}

void Interface::setInitialDirection(const std::string& initialDirection) {
    PRINT("setInitialDirection", initialDirection);
    READ();
}

void Interface::setTileTextNumberOfRows(int tileTextNumberOfRows) {
    PRINT("setTileTextNumberOfRows", tileTextNumberOfRows);
    READ();
}

void Interface::setTileTextNumberOfCols(int tileTextNumberOfCols) {
    PRINT("setTileTextNumberOfCols", tileTextNumberOfCols);
    READ();
}

void Interface::setWheelSpeedFraction(double wheelSpeedFraction) {
    PRINT("setWheelSpeedFraction", wheelSpeedFraction);
    READ();
}

void Interface::finalizeStaticOptions() {
    PRINT("finalizeStaticOptions");
    READ();
}

void Interface::updateAllowOmniscience(bool allowOmniscience) {
    PRINT("updateAllowOmniscience", boolToString(allowOmniscience));
    READ();
}

void Interface::updateAutomaticallyClearFog(bool automaticallyClearFog) {
    PRINT("updateAutomaticallyClearFog", boolToString(automaticallyClearFog));
    READ();
}

void Interface::updateDeclareBothWallHalves(bool declareBothWallHalves) {
    PRINT("updateDeclareBothWallHalves", boolToString(declareBothWallHalves));
    READ();
}

void Interface::updateSetTileTextWhenDistanceDeclared(
        bool setTileTextWhenDistanceDeclared) {
    PRINT("updateSetTileTextWhenDistanceDeclared",
        boolToString(setTileTextWhenDistanceDeclared));
    READ();
}

void Interface::updateSetTileBaseColorWhenDistanceDeclaredCorrectly(
        bool setTileBaseColorWhenDistanceDeclaredCorrectly) {
    PRINT("updateSetTileBaseColorWhenDistanceDeclaredCorrectly",
        boolToString(setTileBaseColorWhenDistanceDeclaredCorrectly));
    READ();
}

void Interface::updateDeclareWallOnRead(bool declareWallOnRead) {
    PRINT("updateDeclareWallOnRead", boolToString(declareWallOnRead));
    READ();
}

void Interface::updateUseTileEdgeMovements(bool useTileEdgeMovements) {
    PRINT("updateUseTileEdgeMovements", boolToString(useTileEdgeMovements));
    READ();
}

int Interface::mazeWidth() {
    PRINT("mazeWidth");
    READ_AND_RETURN_INT();
}

int Interface::mazeHeight() {
    PRINT("mazeHeight");
    READ_AND_RETURN_INT();
}

bool Interface::isOfficialMaze() {
    PRINT("isOfficialMaze");
    READ_AND_RETURN_BOOL();
}

char Interface::initialDirection() {
    PRINT("initialDirection");
    READ_AND_RETURN_CHAR();
}

double Interface::getRandomFloat() {
    PRINT("getRandomFloat");
    READ_AND_RETURN_DOUBLE();
}

int Interface::millis() {
    PRINT("millis");
    READ_AND_RETURN_INT();
}

void Interface::delay(int milliseconds) {
    PRINT("delay", milliseconds);
    READ();
}

void Interface::setTileColor(int x, int y, char color) {
    PRINT("setTileColor", x, y, color);
    READ();
}

void Interface::clearTileColor(int x, int y) {
    PRINT("clearTileColor", x, y);
    READ();
}

void Interface::clearAllTileColor() {
    PRINT("clearAllTileColor");
    READ();
}

void Interface::setTileText(int x, int y, const std::string& text) {
    PRINT("setTileText", x, y, text);
    READ();
}

void Interface::clearTileText(int x, int y) {
    PRINT("clearTileText", x, y);
    READ();
}

void Interface::clearAllTileText() {
    PRINT("clearTileText");
    READ();
}

void Interface::declareWall(int x, int y, char direction, bool wallExists) {
    PRINT("declareWall", x, y, direction, boolToString(wallExists));
    READ();
}

void Interface::undeclareWall(int x, int y, char direction) {
    PRINT("undeclareWall", x, y, direction);
    READ();
}

void Interface::setTileFogginess(int x, int y, bool foggy) {
    PRINT("setTileFogginess", x, y, boolToString(foggy));
    READ();
}

void Interface::declareTileDistance(int x, int y, int distance) {
    PRINT("declareTileDistance", x, y, distance);
    READ();
}

void Interface::undeclareTileDistance(int x, int y) {
    PRINT("undeclareTileDistance", x, y);
    READ();
}

void Interface::resetPosition() {
    PRINT("resetPosition");
    READ();
}

bool Interface::inputButtonPressed(int inputButton) {
    PRINT("inputButtonPressed", inputButton);
    READ_AND_RETURN_BOOL();
}

void Interface::acknowledgeInputButtonPressed(int inputButton) {
    PRINT("acknowledgeInputButtonPressed", inputButton);
    READ();
}

double Interface::getWheelMaxSpeed(const std::string& name) {
    PRINT("getWheelMaxSpeed", name);
    READ_AND_RETURN_DOUBLE();
}

void Interface::setWheelSpeed(const std::string& name, double rpm) {
    PRINT("setWheelSpeed", name, rpm);
    READ();
}

double Interface::getWheelEncoderTicksPerRevolution(const std::string& name) {
    PRINT("getWheelEncoderTicksPerRevolution", name);
    READ_AND_RETURN_DOUBLE();
}

int Interface::readWheelEncoder(const std::string& name) {
    PRINT("readWheelEncoder", name);
    READ_AND_RETURN_INT();
}

void Interface::resetWheelEncoder(const std::string& name) {
    PRINT("resetWheelEncoder", name);
    READ();
}

double Interface::readSensor(const std::string& name) {
    PRINT("readSensor", name);
    READ_AND_RETURN_DOUBLE();
}

double Interface::readGyro() {
    PRINT("readGyro");
    READ_AND_RETURN_DOUBLE();
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

void Interface::moveForward(int count) {
    PRINT("moveForward", count);
    READ();
}

void Interface::turnLeft() {
    PRINT("turnLeft");
    READ();
}

void Interface::turnRight() {
    PRINT("turnRight");
    READ();
}

void Interface::turnAroundLeft() {
    PRINT("turnAroundLeft");
    READ();
}

void Interface::turnAroundRight() {
    PRINT("turnAroundRight");
    READ();
}

void Interface::originMoveForwardToEdge() {
    PRINT("originMoveForwardToEdge");
    READ();
}

void Interface::originTurnLeftInPlace() {
    PRINT("originTurnLeftInPlace");
    READ();
}

void Interface::originTurnRightInPlace() {
    PRINT("originTurnRightInPlace");
    READ();
}

void Interface::moveForwardToEdge() {
    PRINT("moveForwardToEdge");
    READ();
}

void Interface::moveForwardToEdge(int count) {
    PRINT("moveForwardToEdge", count);
    READ();
}

void Interface::turnLeftToEdge() {
    PRINT("turnLeftToEdge");
    READ();
}

void Interface::turnRightToEdge() {
    PRINT("turnRightToEdge");
    READ();
}

void Interface::turnAroundLeftToEdge() {
    PRINT("turnAroundLeftToEdge");
    READ();
}

void Interface::turnAroundRightToEdge() {
    PRINT("turnAroundRightToEdge");
    READ();
}

void Interface::diagonalLeftLeft(int count) {
    PRINT("diagonalLeftLeft", count);
    READ();
}

void Interface::diagonalLeftRight(int count) {
    PRINT("diagonalLeftRight", count);
    READ();
}

void Interface::diagonalRightLeft(int count) {
    PRINT("diagonalRightLeft", count);
    READ();
}

void Interface::diagonalRightRight(int count) {
    PRINT("diagonalRightRight", count);
    READ();
}

int Interface::currentXTile() {
    PRINT("currentXTile");
    READ_AND_RETURN_INT();
}

int Interface::currentYTile() {
    PRINT("currentYTile");
    READ_AND_RETURN_INT();
}

char Interface::currentDirection() {
    PRINT("currentDirection");
    READ_AND_RETURN_CHAR();
}

double Interface::currentXPosMeters() {
    PRINT("currentXPosMeters");
    READ_AND_RETURN_DOUBLE();
}

double Interface::currentYPosMeters() {
    PRINT("currentYPosMeters");
    READ_AND_RETURN_DOUBLE();
}

double Interface::currentRotationDegrees() {
    PRINT("currentRotationDegrees");
    READ_AND_RETURN_DOUBLE();
}

std::string Interface::boolToString(bool value) {
    return value ? "true" : "false";
}
