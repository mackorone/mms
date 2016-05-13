#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "Direction.h"
#include "Maze.h"
#include "units/Duration.h"

namespace sim {

class SimUtilities {

public:

    // The SimUtilities class is not constructible
    SimUtilities() = delete;

    // Quits the simulation
    static void quit();

    // Returns a double in [0.0, 1.0]
    static double getRandom();

    // Sleeps the current thread for ms milliseconds
    static void sleep(const Duration& duration);

    // Like time() in <ctime> but higher resolution (returns seconds since epoch)
    static double getHighResTimestamp();

    // Returns the UTC datetime formatted as YY-MM-DD_HH:MM:SS
    static std::string timestampToDatetimeString(const Duration& timestamp);

    // Converts seconds to mm:ss.ssss... string
    static std::string formatSeconds(double seconds);

    // Convert between types
    static bool isBool(const std::string& str);
    static bool isInt(const std::string& str);
    static bool isDouble(const std::string& str);
    static bool strToBool(const std::string& str);
    static int strToInt(const std::string& str);
    static double strToDouble(const std::string& str);

    // Tokenizes a string on whitespace
    static std::vector<std::string> tokenize(
        const std::string& str,
        char delimiter = ' ',
        bool ignoreEmpties = false,
        bool respectQuotes = false);

    // Returns the string str with the leading and trailing spaces removed
    static std::string trim(const std::string& str);

    // True if path is a path to a file, false otherwise
    static bool isFile(const std::string& path);

    // Returns the index of the direction in DIRECTIONS
    static int getDirectionIndex(Direction direction);

    // Returns a vector of strings of paths of the given directory's contents
    static std::vector<std::string> getDirectoryContents(const std::string& path);

    // Remove oldest runs from the run/ directory if necessary
    static void removeExcessArchivedRuns();

    // A simple pair-comparitor function
    template <class T>
    static bool lessThan(const std::pair<T, T>& one, const std::pair<T, T>& two) {
        return (one.first < two.first ? true : one.second < two.second);
    }

    // A simple pair-retrieval function
    template <class T>
    static std::pair<T, T> min(const std::vector<std::pair<T, T>>& pairs) {
        return *std::min_element(pairs.begin(), pairs.end(), lessThan<T>);
    }

};

} // namespace sim
