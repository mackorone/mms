#pragma once

#include <GL/freeglut.h>
#include <string>
#include <vector>

#include <Time.h>

namespace sim {

// Quits the simulation
void quit();

// Prints a message to stdout
void print(const std::string& msg);

// Returns a float in [0.0, 1.0]
float getRandom();

// Sleeps the current thread for ms milliseconds
void sleep(const Time& time);

// Like time() in <ctime> but higher resolution
double getHighResTime();

// Returns the path the mms project directory
std::string getProjectDirectory();

// Convert between types
bool isBool(std::string str);
bool isInt(std::string str);
bool isFloat(std::string str);
bool strToBool(std::string str);
int strToInt(std::string str);
float strToFloat(std::string str);

// Tokenizes a string on whitespace
std::vector<std::string> tokenize(std::string str);

// True if path is a path to a file, false otherwise
bool isFile(std::string path);

} // namespace sim
