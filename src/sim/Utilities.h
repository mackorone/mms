#pragma once

#include <cstdlib> 
#include <GL/freeglut.h>
#include <iostream> 
#include <string>
#include <vector>

#include "Param.h"
#include "Polygon.h"
#include "units/Cartesian.h"
#include "units/Time.h"

namespace sim {

// -------------------------- Backend Utilities -------------------------------

// Asserts that a condition is true, program terminates if not
#define ASSERT(condition) \
if ( !(condition) ) { \
    std::cout << "---------- Assertion failed! ----------" << std::endl \
              << "FILE: " << __FILE__   << std::endl \
              << "LINE: " << __LINE__   << std::endl \
              << "COND: " << #condition << std::endl \
              << "---------------------------------------" << std::endl; \
    exit(1); \
}

// Sleeps the current thread for ms milliseconds
void sleep(const Time& time);

// Returns the path the mms project directory
std::string getProjectDirectory();

// Ensures that str contains characters corresponding to a particular primitive type
bool isBool(std::string str);
bool isInt(std::string str);
bool isFloat(std::string str);

// Convert between types
bool strToBool(std::string str);
int strToInt(std::string str);
float strToFloat(std::string str);

// Tokenizes a string on whitespace
std::vector<std::string> tokenize(std::string str);

// TODO: These can get refactored
bool checkValidMazeFileTom(std::string mazeFilePath, int height, int width);
bool hasOnlyDigits(const std::string& str);


// -------------------------- Graphics Utilities ------------------------------

// Converts points from physical coordinates to OpenGl coordinates (bottem-left is (0,0)) to the
// OpenGL specification
Cartesian physicalToOpenGl(const Coordinate& point);
Polygon physicalToOpenGl(const Polygon& polygon);

// Helpers... TODO: refactor these...
// OpenGL specification
float convertHorizontalPoint(float coordinate); // TODO - Better interface...
float convertHorizontalPoint(float coordinate); // TODO - Better interface...
float convertVerticalPoint(float coordinate); // TODO - Better interface...
float rtp(float coordinate); // Real coordinate to pixel coordinate

void drawPolygon(const Polygon& polygon);

// TODO
bool mackValid(std::string mazeFilePath);
std::pair<int, int> getMazeWidthAndHeight(std::string mazeFilePath);

} // namespace sim
