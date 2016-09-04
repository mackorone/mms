#include "Interface.h"

int Interface::mazeWidth() {
}

int Interface::mazeHeight() {
}

bool Interface::isOfficialMaze() {
}

char Interface::initialDirection() {
}

double Interface::getRandomFloat() {
}

int Interface::millis() {
}

void Interface::delay(int milliseconds) {
}

void Interface::setTileColor(int x, int y, char color) {
}

void Interface::clearTileColor(int x, int y) {
}

void Interface::clearAllTileColor() {
}

void Interface::setTileText(int x, int y, const std::string& text) {
}

void Interface::clearTileText(int x, int y) {
}

void Interface::clearAllTileText() {
}

void Interface::declareWall(int x, int y, char direction, bool wallExists) {
}

void Interface::undeclareWall(int x, int y, char direction) {
}

void Interface::setTileFogginess(int x, int y, bool foggy) {
}

void Interface::declareTileDistance(int x, int y, int distance) {
}

void Interface::undeclareTileDistance(int x, int y) {
}

void Interface::resetPosition() {
}

bool Interface::inputButtonPressed(int inputButton) {
}

void Interface::acknowledgeInputButtonPressed(int inputButton) {
}

double Interface::getWheelMaxSpeed(const std::string& name) {
}

void Interface::setWheelSpeed(const std::string& name, double rpm) {
}

double Interface::getWheelEncoderTicksPerRevolution(const std::string& name) {
}

int Interface::readWheelEncoder(const std::string& name) {
}

void Interface::resetWheelEncoder(const std::string& name) {
}

double Interface::readSensor(const std::string& name) {
}

double Interface::readGyro() {
}

bool Interface::wallFront() {
}

bool Interface::wallRight() {
}

bool Interface::wallLeft() {
}

void Interface::moveForward() {
}

void Interface::moveForward(int count) {
}

void Interface::turnLeft() {
}

void Interface::turnRight() {
}

void Interface::turnAroundLeft() {
}

void Interface::turnAroundRight() {
}

void Interface::originMoveForwardToEdge() {
}

void Interface::originTurnLeftInPlace() {
}

void Interface::originTurnRightInPlace() {
}

void Interface::moveForwardToEdge() {
}

void Interface::moveForwardToEdge(int count) {
}

void Interface::turnLeftToEdge() {
}

void Interface::turnRightToEdge() {
}

void Interface::turnAroundLeftToEdge() {
}

void Interface::turnAroundRightToEdge() {
}

void Interface::diagonalLeftLeft(int count) {
}

void Interface::diagonalLeftRight(int count) {
}

void Interface::diagonalRightLeft(int count) {
}

void Interface::diagonalRightRight(int count) {
}

int Interface::currentXTile() {
}

int Interface::currentYTile() {
}

char Interface::currentDirection() {
}

double Interface::currentXPosMeters() {
}

double Interface::currentYPosMeters() {
}

double Interface::currentRotationDegrees() {
}
