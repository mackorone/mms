#pragma once

#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE

#include <easylogging++.h>

#define WARN WARNING

#define LOG_STRING "log"
#define PRINT_STRING "print"

#undef LOG
#define LOG(LEVEL) CLOG(LEVEL, LOG_STRING)
#define PRINT(LEVEL) CLOG(LEVEL, PRINT_STRING)

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

    // A private constructor so we can't make instances...
    Logging(); // TODO: MACK - Logging = delete (do this in other places too! SimUtilties, etc.)
};

} // namespace sim
