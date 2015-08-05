#pragma once

#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE
#include <easylogging++.h>
#define WARN WARNING

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
