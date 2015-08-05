#include "Logging.h"

#include <cstdio>

#include "Assert.h"
#include "SimUtilities.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

// Create and initialize the static variables
std::string Logging::m_runId = "";
int Logging::m_numLogFiles = 0;

void Logging::initialize(const std::string& runId) {

    // Ensure we only initialize the loggers once
    static bool initialized = false;
    ASSERT(!initialized);
    initialized = true;

    // Set the runId
    m_runId = runId;

    // TODO: MACK - Configure each of the logger types here...
    // Register and configure the default logger
    el::Configurations config;
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    config.setGlobally(el::ConfigurationType::Filename,
        SimUtilities::getProjectDirectory() + "run/" + m_runId + "/logs/last.txt");
    config.setGlobally(el::ConfigurationType::MaxLogFileSize,
        std::to_string(10 * 1024 * 1024)); // 10 MB, ~10,000 lines
    config.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
    config.setGlobally(el::ConfigurationType::Format,
        "%datetime{%Y-%M-%d %H:%m:%s.%g} [%level] %loc - %msg");
    el::Loggers::reconfigureAllLoggers(config);
    el::Helpers::installPreRollOutCallback(rolloutHandler);
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
