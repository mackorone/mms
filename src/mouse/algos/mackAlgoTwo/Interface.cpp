#include "Interface.h"

#include "Options.h"
#include "Printer.h"
#include "Reader.h"

#if (!SIMULATOR) 
extern bool walls_global[3];
extern volatile bool buttonPressed;
#endif

void Interface::useContinuousInterface() {
#if (SIMULATOR)
    PRINT("useContinuousInterface");
    READ();
#endif
}

void Interface::setInitialDirection(char initialDirection) {
#if (SIMULATOR)
    PRINT("setInitialDirection", initialDirection);
    READ();
#endif
}

void Interface::setTileTextRowsAndCols(int numRows, int numCols) {
#if (SIMULATOR)
    PRINT("setTileTextRowsAndCols", numRows, numCols);
    READ();
#endif
}

void Interface::setWheelSpeedFraction(double wheelSpeedFraction) {
#if (SIMULATOR)
    PRINT("setWheelSpeedFraction", wheelSpeedFraction);
    READ();
#endif
}

void Interface::updateAllowOmniscience(bool allowOmniscience) {
#if (SIMULATOR)
    PRINT("updateAllowOmniscience", boolToString(allowOmniscience));
    READ();
#endif
}

void Interface::updateAutomaticallyClearFog(bool automaticallyClearFog) {
#if (SIMULATOR)
    PRINT("updateAutomaticallyClearFog", boolToString(automaticallyClearFog));
    READ();
#endif
}

void Interface::updateDeclareBothWallHalves(bool declareBothWallHalves) {
#if (SIMULATOR)
    PRINT("updateDeclareBothWallHalves", boolToString(declareBothWallHalves));
    READ();
#endif
}

void Interface::updateSetTileTextWhenDistanceDeclared(
        bool setTileTextWhenDistanceDeclared) {
#if (SIMULATOR)
    PRINT("updateSetTileTextWhenDistanceDeclared",
        boolToString(setTileTextWhenDistanceDeclared));
    READ();
#endif
}

void Interface::updateSetTileBaseColorWhenDistanceDeclaredCorrectly(
        bool setTileBaseColorWhenDistanceDeclaredCorrectly) {
#if (SIMULATOR)
    PRINT("updateSetTileBaseColorWhenDistanceDeclaredCorrectly",
        boolToString(setTileBaseColorWhenDistanceDeclaredCorrectly));
    READ();
#endif
}

void Interface::updateDeclareWallOnRead(bool declareWallOnRead) {
#if (SIMULATOR)
    PRINT("updateDeclareWallOnRead", boolToString(declareWallOnRead));
    READ();
#endif
}

void Interface::updateUseTileEdgeMovements(bool useTileEdgeMovements) {
#if (SIMULATOR)
    PRINT("updateUseTileEdgeMovements", boolToString(useTileEdgeMovements));
    READ();
#endif
}

int Interface::mazeWidth() {
#if (SIMULATOR)
    PRINT("mazeWidth");
    READ_AND_RETURN_INT();
#else
    return 16;
#endif
}

int Interface::mazeHeight() {
#if (SIMULATOR)
    PRINT("mazeHeight");
    READ_AND_RETURN_INT();
#else
    return 16;
#endif
}

bool Interface::isOfficialMaze() {
#if (SIMULATOR)
    PRINT("isOfficialMaze");
    READ_AND_RETURN_BOOL();
#else
    return true;
#endif
}

char Interface::initialDirection() {
#if (SIMULATOR)
    PRINT("initialDirection");
    READ_AND_RETURN_CHAR();
#else
    return 'n';
#endif
}

double Interface::getRandomFloat() {
#if (SIMULATOR)
    PRINT("getRandomFloat");
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

int Interface::millis() {
#if (SIMULATOR)
    PRINT("millis");
    READ_AND_RETURN_INT();
#else
    return 0;
#endif
}

void Interface::delay(int milliseconds) {
#if (SIMULATOR)
    PRINT("delay", milliseconds);
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

void Interface::setTileFogginess(int x, int y, bool foggy) {
#if (SIMULATOR)
    PRINT("setTileFogginess", x, y, boolToString(foggy));
#endif
}

void Interface::declareTileDistance(int x, int y, int distance) {
#if (SIMULATOR)
    PRINT("declareTileDistance", x, y, distance);
#endif
}

void Interface::undeclareTileDistance(int x, int y) {
#if (SIMULATOR)
    PRINT("undeclareTileDistance", x, y);
#endif
}

void Interface::resetPosition() {
#if (SIMULATOR)
    PRINT("resetPosition");
#endif
}

bool Interface::inputButtonPressed(int inputButton) {
#if (SIMULATOR)
    PRINT("inputButtonPressed", inputButton);
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

double Interface::getWheelMaxSpeed(const std::string& name) {
#if (SIMULATOR)
    PRINT("getWheelMaxSpeed", name);
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

void Interface::setWheelSpeed(const std::string& name, double rpm) {
#if (SIMULATOR)
    PRINT("setWheelSpeed", name, rpm);
    READ();
#endif
}

double Interface::getWheelEncoderTicksPerRevolution(const std::string& name) {
#if (SIMULATOR)
    PRINT("getWheelEncoderTicksPerRevolution", name);
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

int Interface::readWheelEncoder(const std::string& name) {
#if (SIMULATOR)
    PRINT("readWheelEncoder", name);
    READ_AND_RETURN_INT();
#else
    return 0;
#endif
}

void Interface::resetWheelEncoder(const std::string& name) {
#if (SIMULATOR)
    PRINT("resetWheelEncoder", name);
    READ();
#endif
}

double Interface::readSensor(const std::string& name) {
#if (SIMULATOR)
    PRINT("readSensor", name);
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

double Interface::readGyro() {
#if (SIMULATOR)
    PRINT("readGyro");
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

bool Interface::wallFront() {
#if (SIMULATOR)
    PRINT("wallFront");
    READ_AND_RETURN_BOOL();
#else
    return walls_global[1];
#endif
}

bool Interface::wallRight() {
#if (SIMULATOR)
    PRINT("wallRight");
    READ_AND_RETURN_BOOL();
#else
    return walls_global[2];
#endif
}

bool Interface::wallLeft() {
#if (SIMULATOR)
    PRINT("wallLeft");
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

void Interface::moveForward(int count) {
#if (SIMULATOR)
    PRINT("moveForward", count);
    READ();
#endif
}

void Interface::turnLeft() {
#if (SIMULATOR)
    PRINT("turnLeft");
    READ();
#endif
}

void Interface::turnRight() {
#if (SIMULATOR)
    PRINT("turnRight");
    READ();
#endif
}

void Interface::turnAroundLeft() {
#if (SIMULATOR)
    PRINT("turnAroundLeft");
    READ();
#endif
}

void Interface::turnAroundRight() {
#if (SIMULATOR)
    PRINT("turnAroundRight");
    READ();
#endif
}

void Interface::originMoveForwardToEdge() {
#if (SIMULATOR)
    PRINT("originMoveForwardToEdge");
    READ();
#endif
}

void Interface::originTurnLeftInPlace() {
#if (SIMULATOR)
    PRINT("originTurnLeftInPlace");
    READ();
#endif
}

void Interface::originTurnRightInPlace() {
#if (SIMULATOR)
    PRINT("originTurnRightInPlace");
    READ();
#endif
}

void Interface::moveForwardToEdge() {
#if (SIMULATOR)
    PRINT("moveForwardToEdge");
    READ();
#endif
}

void Interface::moveForwardToEdge(int count) {
#if (SIMULATOR)
    PRINT("moveForwardToEdge", count);
    READ();
#endif
}

void Interface::turnLeftToEdge() {
#if (SIMULATOR)
    PRINT("turnLeftToEdge");
    READ();
#endif
}

void Interface::turnRightToEdge() {
#if (SIMULATOR)
    PRINT("turnRightToEdge");
    READ();
#endif
}

void Interface::turnAroundLeftToEdge() {
#if (SIMULATOR)
    PRINT("turnAroundLeftToEdge");
    READ();
#endif
}

void Interface::turnAroundRightToEdge() {
#if (SIMULATOR)
    PRINT("turnAroundRightToEdge");
    READ();
#endif
}

void Interface::diagonalLeftLeft(int count) {
#if (SIMULATOR)
    PRINT("diagonalLeftLeft", count);
    READ();
#endif
}

void Interface::diagonalLeftRight(int count) {
#if (SIMULATOR)
    PRINT("diagonalLeftRight", count);
    READ();
#endif
}

void Interface::diagonalRightLeft(int count) {
#if (SIMULATOR)
    PRINT("diagonalRightLeft", count);
    READ();
#endif
}

void Interface::diagonalRightRight(int count) {
#if (SIMULATOR)
    PRINT("diagonalRightRight", count);
    READ();
#endif
}

int Interface::currentXTile() {
#if (SIMULATOR)
    PRINT("currentXTile");
    READ_AND_RETURN_INT();
#else
    return 0;
#endif
}

int Interface::currentYTile() {
#if (SIMULATOR)
    PRINT("currentYTile");
    READ_AND_RETURN_INT();
#else
    return 0;
#endif
}

char Interface::currentDirection() {
#if (SIMULATOR)
    PRINT("currentDirection");
    READ_AND_RETURN_CHAR();
#else
    return '\0';
#endif
}

double Interface::currentXPosMeters() {
#if (SIMULATOR)
    PRINT("currentXPosMeters");
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

double Interface::currentYPosMeters() {
#if (SIMULATOR)
    PRINT("currentYPosMeters");
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

double Interface::currentRotationDegrees() {
#if (SIMULATOR)
    PRINT("currentRotationDegrees");
    READ_AND_RETURN_DOUBLE();
#else
    return 0.0;
#endif
}

std::string Interface::boolToString(bool value) {
    return value ? "true" : "false";
}
