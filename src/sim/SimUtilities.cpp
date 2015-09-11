#include "SimUtilities.h"

#include <chrono>
#include <ctime>
#include <ratio>
#include <fstream>
#include <glut/glut.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <random>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "units/Seconds.h"

#include "Assert.h"
#include "Param.h"
#include "State.h"
#include "Logging.h"

namespace sim {

void SimUtilities::quit() {

    // The reason we don't just call exit() is because sometimes the window
    // wasn't reclaimed properly. So we use glutLeaveMainLoop, but only after
    // we've actually entered the main loop.

    if (!S()->mainLoopEntered()) {
        exit(0);
    }

    // If we're the main thread, exit the main loop
    if (std::this_thread::get_id() == S()->mainThreadId()) {
        // TODO: MACK - get rid of this as a dependency here...
        glutLeaveMainLoop(); 
    }

    // If we're not the main thread, just sleep until the program terminates
    else {
        while (true) {
            sleep(Seconds(1)); 
        }
    }
}

void SimUtilities::print(const std::string& msg) {
    std::cout << msg << std::endl;
}

double SimUtilities::getRandom() {
    
    // The '- 1' ensures that the random number is never 1.
    // This matches the python implementation where random is [0,1).
    // This is particularly useful if you want to index into array like so
    // array[std::floor(random * <number of elements>)] without having to check
    // the condition if this function returns 1.
    
    static std::mt19937 generator(sim::P()->randomSeed());
    return std::abs(static_cast<double>(generator()) - 1) / static_cast<double>(generator.max());
}

void SimUtilities::sleep(const Duration& duration) {
    ASSERT(duration.getMilliseconds() >= 0);
	std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(duration.getMicroseconds())));
}

double SimUtilities::getHighResTime() {
#ifdef __linux
    // chrono::high_resoltion_clock as defined by c++ 11 should use the highest resolution time source available
    // on the system.  See below for windows note.
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    // By default a cast to duration is a cast to seconds since the default ratio is 1
    return std::chrono::duration_cast<std::chrono::duration<double>>(t1.time_since_epoch()).count();
#elif _WIN32
    LARGE_INTEGER freq, counter;         // Keep windows queryperformance for the time being until tests can
    QueryPerformanceFrequency(&freq);    // be done. Supposedly up until Visual Studio 2013, microsoft uses
    QueryPerformanceCounter(&counter);   // the normal system clock for chrono high_resolution_clock which only
    return double(counter.QuadPart) / double(freq.QuadPart); // has 1 ms accuracy.
#endif
}

std::string SimUtilities::getDateTime() {
    // Taken from http://stackoverflow.com/a/10467633
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *gmtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

bool SimUtilities::isBool(const std::string& str) {
    return 0 == str.compare("true") || 0 == str.compare("false");
}

bool SimUtilities::isInt(const std::string& str) {
    try {
        std::stoi(str);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool SimUtilities::isDouble(const std::string& str) {
    try {
        std::stod(str);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool SimUtilities::strToBool(const std::string& str) {
    ASSERT(isBool(str));
    return 0 == str.compare("true");
}

int SimUtilities::strToInt(const std::string& str) {
    ASSERT(isInt(str));
    return std::stoi(str.c_str());
}

double SimUtilities::strToDouble(const std::string& str) {
    ASSERT(isDouble(str));
    return std::stof(str);
}


std::vector<std::string> SimUtilities::tokenize(const std::string& str, bool respectQuotes) {

    std::vector<std::string> tokens;
    std::string word = "";

    for (int i = 0; i < str.size(); ++i) {
        if (respectQuotes && str.at(i) == '\"') {
            do {
                word += str.at(i);
                i += 1;
            } while (i < str.size() && str.at(i) != '\"');
        }
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

std::string SimUtilities::trim(const std::string& str) {
    std::size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    std::size_t last  = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}

bool SimUtilities::isFile(const std::string& path) {
    std::ifstream infile(path);
    return infile.good();
}

int SimUtilities::getDirectionIndex(Direction direction) {
    return std::find(DIRECTIONS.begin(), DIRECTIONS.end(), direction) - DIRECTIONS.begin();
}

void SimUtilities::removeExcessArchivedRuns() {
    // TODO: upforgrabs
    // Information about each run is stored in the run/ directory. As it turns
    // out, this information can pile up pretty quickly. We should remove the
    // oldest stuff so that the run/ directory doesn't get too full. In
    // particular, supposing that there are N runs in the run/ directory, and
    // supposing N >= P()->numberOfArchivedRuns(), implement this function to
    // remove the (N - P()->numberOfArchivedRuns() + 1) oldest directories in
    // run/.  Make sure that your solution is cross-platform, and LOG where
    // appropriate (e.g., if, for some reason, we can't delete one of the
    // directories in run/). This function is already called in the appropriate
    // place, so once you implement it, it should "just work".
    // Hint: Use std::remove
}

} // namespace sim
