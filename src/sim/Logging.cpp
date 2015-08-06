#include "Logging.h"

#include <cstdio>

#include "Assert.h"
#include "SimUtilities.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

// Create and initialize the static variables
el::Logger* Logging::m_logger = nullptr;
el::Logger* Logging::m_printer = nullptr;
std::string Logging::m_runId = "";
int Logging::m_numLogFiles = 0;

el::Logger* Logging::logger() {
    ASSERT(m_logger != NULL);
    return m_logger;
}

el::Logger* Logging::printer() {
    ASSERT(m_printer != NULL);
    return m_printer;
}

void Logging::initialize(const std::string& runId) {

    // Ensure we only initialize the loggers once
    static bool initialized = false;
    ASSERT(!initialized);
    initialized = true;

    // Set the runId
    m_runId = runId;

    // Register and configure the logger
    el::Configurations logConfig;
    m_logger = el::Loggers::getLogger(LOG_STRING);
    logConfig.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    logConfig.setGlobally(el::ConfigurationType::Filename,
        SimUtilities::getProjectDirectory() + "run/" + m_runId + "/logs/last.txt");
    logConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
        std::to_string(10 * 1024 * 1024)); // 10 MiB, ~10,000 lines
    logConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
    logConfig.setGlobally(el::ConfigurationType::Format,
        "%datetime{%Y-%M-%d %H:%m:%s.%g} [%level] %msg");
    el::Loggers::reconfigureLogger(LOG_STRING, logConfig);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Helpers::installPreRollOutCallback(rolloutHandler);

    // Register and configure the printer
    el::Configurations printConfig;
    m_printer = el::Loggers::getLogger(PRINT_STRING);
    printConfig.setGlobally(el::ConfigurationType::ToFile, "false");
    printConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
    printConfig.setGlobally(el::ConfigurationType::Format, "[%level] - %msg");
    el::Loggers::reconfigureLogger(PRINT_STRING, printConfig);
}

std::string Logging::getNextLogFileName() {
    m_numLogFiles += 1;
    return SimUtilities::getProjectDirectory() + "run/" + m_runId
        + "/logs/" + std::to_string(m_numLogFiles) + ".txt";
}

void Logging::rolloutHandler(const char* filename, std::size_t size) {
    int value = std::rename(filename, getNextLogFileName().c_str());
    ASSERT_EQUAL(value, 0);
}

} // namespace sim
