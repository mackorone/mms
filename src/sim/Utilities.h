#pragma once

#include <cstdlib> 
#include <iostream> 
#include <string>

#include "Param.h"

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
    exit(1);\
}

// Sleeps the current thread for ms milliseconds
void sleep(int ms);

// Returns the path the mms project directory
std::string getProjectDirectory();

// TODO: These can get refactored
bool checkValidMazeFileTom(std::string mazeFilePath, int height, int width);
bool hasOnlyDigits(const std::string& str);


// -------------------------- Graphics Utilities ------------------------------

// Converts points from pixel coordinates (bottem-left is (0,0)) to the
// OpenGL specification

float convertHorizontalPoint(float coordinate); // TODO - Better interface...
float convertVerticalPoint(float coordinate); // TODO - Better interface...

} // namespace sim
