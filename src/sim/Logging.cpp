#include "Logging.h"

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

// Create and initialize the static variables
std::string Logging::m_runId = "";
std::string Logging::m_simLoggerName = "sim";
std::string Logging::m_mazeLoggerName = "maze";
std::string Logging::m_mouseLoggerName = "mouse";
std::map<std::string, std::pair<std::string, int>> Logging::m_info;

el::Logger* Logging::simLogger() {
    return getLogger(m_simLoggerName);
}

el::Logger* Logging::mazeLogger() {
    return getLogger(m_mazeLoggerName);
}

el::Logger* Logging::mouseLogger() {
    return getLogger(m_mouseLoggerName);
}

void Logging::initialize(const std::string& runId) {

    // TODO: MACK - initialize the maze and mouse loggers after reading the params so that we can decide if we want to log at runtime
    // TODO: MACK - is there a way to print only above a certain level? runtime param, perhaps?
    // TODO: MACK - get the high res time, set up the custom formatter thingy

    // Ensure we only initialize the loggers once
    static bool initialized = false;
    ASSERT(!initialized);
    initialized = true;

    // Set the runId
    m_runId = runId;

    // For each of the logger names ...
    for (std::string loggerName : {m_simLoggerName, m_mazeLoggerName, m_mouseLoggerName}) {
        
        // ... create the logger ...
        el::Logger* logger = el::Loggers::getLogger(loggerName);
        std::string loggerPath = Directory::getRunDirectory() + m_runId + "/" + loggerName + "/default.txt";
        m_info.insert(std::make_pair(loggerName, std::make_pair(loggerPath, 1)));

        // ... and configure it
        el::Configurations loggerConfig;
        loggerConfig.setGlobally(el::ConfigurationType::Filename, loggerPath); // TODO: MACK - surround with if-checks to prevent logging
        loggerConfig.setGlobally(el::ConfigurationType::ToStandardOutput, "true"); // TODO: MACK Potentially configurable for maze and mouse
        loggerConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
            std::to_string(10 * 1024 * 1024)); // 10 MiB, ~10,000 lines
        loggerConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
        loggerConfig.setGlobally(el::ConfigurationType::Format,
            "[%datetime{%Y-%M-%d %H:%m:%s.%g}] # %logger # (%level) - %msg"); // TODO: Don't print the datetime, instead print elapsed real/sim time
        el::Loggers::reconfigureLogger(logger, loggerConfig);
    }

    // Set some flags and the rollout callback
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Helpers::installPreRollOutCallback(rolloutHandler);
}

el::Logger* Logging::getLogger(const std::string& loggerName) {
    ASSERT(SimUtilities::mapContains(m_info, loggerName)); // TODO: MACK - is this the best assert??
    return el::Loggers::getLogger(loggerName);
}

std::string Logging::getNextFileName(const char* filename) {
    std::string path = "";
    // TODO: MACK - Does this even work???
    for (auto logger : m_info) {
        /*
        std::string loggerPath = logger.second.second.first;
        if (std::string(filename) == loggerPath) {
            m_info.insert(std::make_pair(logger.first,
                std::make_pair(logger.second.first, logger.second.second.first, logger.second.second.second + 1)));
            path = "/" + logger.first "/" + std::to_string(logger.second.second.second) + ".txt";
        }
        */
    }
    return Directory::getRunDirectory() + m_runId + path;
}

void Logging::rolloutHandler(const char* filename, std::size_t size) {
    int value = std::rename(filename, getNextFileName(filename).c_str());
    ASSERT_EQUAL(value, 0);
}

} // namespace sim
