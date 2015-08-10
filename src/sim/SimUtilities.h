#pragma once

#include <string>
#include <vector>

#include <glut.h>
#include "units/Duration.h"

#include "Directions.h"
#include "Maze.h"

namespace sim {

class SimUtilities {

public:

    // The SimUtilities class is not constructible
    SimUtilities() = delete;

    // Quits the simulation
    static void quit();

    // TODO: upforgrabs
    // We recently adopted a logging system for the simulator, which can give
    // us much more context and information about messages. We should no longer
    // use the SimUtilities::print function. Your mission, should you choose to
    // accept it, is to replace, everywhere in the code, SimUtilities::print
    // with an appropriate PRINT or IF_PRINT_ELSE_LOG. Be mindful of the
    // severity (DEBUG, INFO, WARN, ERROR, FATAL) that you use for log
    // statements, and make sure to format the messages correctly (keeping
    // whitespace in mind). Once you've replaced all of the
    // SimUtilities::print, remove the print function from here and from
    // SimUtilities.cpp. See the end of the draw() function in Main.cpp for an
    // example of how to use the IF_PRINT_ELSE_LOG macro.

    // Prints a message to stdout
    static void print(const std::string& msg);

    // Returns a double in [0.0, 1.0]
    static double getRandom();

    // Sleeps the current thread for ms milliseconds
    static void sleep(const Duration& duration);

    // Like time() in <ctime> but higher resolution
    static double getHighResTime();

    // Returns the UTC datetime formatted as YY-MM-DD_HH:MM:SS
    static std::string getDateTime();

    // Returns the path the mms project directory
    static std::string getProjectDirectory();

    // Convert between types
    static bool isBool(const std::string& str);
    static bool isInt(const std::string& str);
    static bool isDouble(const std::string& str);
    static bool strToBool(const std::string& str);
    static int strToInt(const std::string& str);
    static double strToDouble(const std::string& str);

    // Tokenizes a string on whitespace
    static std::vector<std::string> tokenize(const std::string& str, bool respectQuotes);

    // Returns the string str with the leading and trailing spaces removed
    static std::string trim(const std::string& str);

    // True if path is a path to a file, false otherwise
    static bool isFile(const std::string& path);

    // Returns the index of the direction in DIRECTIONS
    static int getDirectionIndex(Direction direction);

    // Remove oldest runs from the run/ directory if necessary
    static void removeExcessArchivedRuns();

    // Returns whether or not a map contains a particular key
    template<class K, class V>
    static bool mapContains(const std::map<K, V>& map, K key) {
        return map.find(key) != map.end();
    }

};

} // namespace sim
