#include "SimUtilities.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <thread>

#ifdef __linux
    #include <limits.h>
    #include <sys/time.h>
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif

#include "Assert.h"
#include "Param.h"

namespace sim {

void quit() {
    exit(0);
}

void print(const std::string& msg) {
    std::cout << msg << std::endl;
}

float getRandom() {
#ifdef __linux
    static bool seeded = false;
    if (!seeded) {
        srand(sim::P()->randomSeed());
        seeded = true;
    }
    return rand() / static_cast<float>(RAND_MAX);
#elif _WIN32
    // TODO: Tomasz said he'd implement the Windows version
    return 0.0;
#endif
}

void sleep(const Time& time) {
    ASSERT(time.getMilliseconds() >= 0);
	std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(time.getMicroseconds())));
}

double getHighResTime() {
    // TODO: SOM - I tried to reimplement w/ chrono but without much succes...
    // TODO: That is the best option, if it works
#ifdef __linux
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + (t.tv_usec/1000000.0);
#elif _WIN32
    // TODO: SOM - See QueryPerformanceCounter
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return double(counter.QuadPart)/double(freq.QuadPart);
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
    try {
        std::stoi(str);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool isFloat(std::string str) {
    try {
        std::stof(str);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool strToBool(std::string str) {
    ASSERT(isBool(str));
    return 0 == str.compare("true");
}

int strToInt(std::string str) {
    ASSERT(isInt(str));
    return std::stoi(str.c_str());
}

float strToFloat(std::string str) {
    ASSERT(isFloat(str));
    return std::stof(str);
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

bool isFile(std::string path) {
    struct stat buf;
    stat(path.c_str(), &buf);
    return S_ISREG(buf.st_mode);
}

} // namespace sim
