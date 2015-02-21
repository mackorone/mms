#include "SimUtilities.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <thread>

#ifdef __linux
    #include <limits.h>
    #include <sys/time.h>
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif

#include "Assert.h"

namespace sim {

void print(const std::string& msg) {
    std::cout << msg << std::endl;
}

void sleep(const Time& time) {
    ASSERT(time.getMilliseconds() >= 0);
	std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(time.getMicroseconds())));
}

double getHighResTime() {
    // TODO: I tried to reimplement w/ chrono but without much succes...
#ifdef __linux
    struct timeval t;  
    gettimeofday(&t, NULL);  
    return t.tv_sec + (t.tv_usec/1000000.0);  
#elif _WIN32
    // TODO: SOM, see QueryPerformanceCounter
    return 0.0;
#endif
}

std::string getProjectDirectory() {

    // This approach is claimed to be more reliable than argv[0] on windows
    // and linux.  On Windows GetModuleFileName is the directory to the executable
    // which is located in /mms/src/Debug/.  On linux /proc/self/exe is a path to exe.
    // This approach does not work for all flavors to Linux, but should work on most
    // common ones. The executable on Linux is located in "mms/bin".

    std::string path;

#ifdef __linux
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    path = std::string(result, (count > 0) ? count : 0);
    path = path.substr(0, path.find_last_of("\\/"));; // Remove the executable name as it is part of path
    path += "/../";
#elif _WIN32
    char result[MAX_PATH];
    path = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
    path = path.substr(0, path.find_last_of("\\/"));; // Remove the executable name as it is part of path
    path += "\\..\\..\\"; // Point to /mms/
    // Windows uses \ in directory
#endif

    return path;
}

bool isBool(std::string str) {
    return 0 == str.compare("true") || 0 == str.compare("false");
}

bool isInt(std::string str) {
    
    // If there's a negative sign, remove it
    if (!str.empty() && '-' == str.at(0)) {
        str = str.substr(1, str.size() - 1);
    }

    // Ensure it's not an empty string
    if (str.empty()) {
        return false;
    }

    // Ensure there are no leading zeros
    if ('0' == str.at(0)) {
        return 1 == str.size();
    }

    // Ensure that we have all digits
    for (int i = 0; i < str.size(); i += 1) {
        if (!('0' <= str.at(i) && str.at(i) <= '9')) {
            return false;
        }
    }

    return true;
}

bool isFloat(std::string str) {
    // TODO: The param parser won't work quite yet without this...
    // TODO: SOM
    return false;
}

bool strToBool(std::string str) {
    ASSERT(isBool(str));
    return 0 == str.compare("true");
}

int strToInt(std::string str) {
    ASSERT(isInt(str));
    return atoi(str.c_str());
}

float strToFloat(std::string str) {
    ASSERT(isFloat(str));
    return stof(str); // TODO: SOM - confirm that this is correct
}


std::vector<std::string> tokenize(std::string str) {

    std::vector<std::string> tokens;
    std::string word = ""; 

    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == ' ') {
            if (!word.empty()) {
                tokens.push_back(word);
                word = ""; 
            }   
        }   
        else {
            word += str.at(i);
        }   
    }   
    if (!word.empty()) {
        tokens.push_back(word);
        word = ""; 
    }   

    return tokens;
}

} // namespace sim
