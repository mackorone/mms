#pragma once

#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE
#include <easylogging++.h>

#define MAZE_STRING "maze"
#define MOUSE_STRING "mouse"
#define SIM_STRING "sim"

// TODO: Logging by class, not by section...
#define LOG_MAZE(LEVEL) CLOG(LEVEL, MAZE_STRING)
#define LOG_MOUSE(LEVEL) CLOG(LEVEL, MOUSE_STRING)
#define LOG_SIM(LEVEL) CLOG(LEVEL, SIM_STRING)

/*
#define L(INFO, ...)\
el::Logger* logger = el::Loggers::getLogger(__FILE__);\
logger->info(__VA_ARGS__);
*/

namespace sim {

class Logging {

public:

    // Initializes all of the loggers, should only be called once
    static void initialize(const std::string& runId);

private:

    // Used to determine the log file paths
    static std::string m_runId;

    // Keep track of the number of log files, so we can make more if necessary
    static int m_numLogFiles;

    // Easy function for getting the next available log file name
    static std::string getNextLogFileName();

    // Perform an action when files get too large
    static void rolloutHandler(const char* filename, std::size_t size);

    // A private constructor is used to ensure only one instance of this class exists
    Logging();
};

} // namespace sim
