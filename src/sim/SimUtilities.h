#pragma once

#include <GL/freeglut.h>
#include <string>
#include <vector>

#include <Time.h>

namespace sim {

// Prints a message to stdout
void print(const std::string& msg);

// Sleeps the current thread for ms milliseconds
void sleep(const Time& time);

// Like time() in <ctime> but higher resolution
double getHighResTime();

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

} // namespace sim
