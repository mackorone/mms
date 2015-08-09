#include "Logging.h"

#include <cstdio>

#include "Assert.h"
#include "SimUtilities.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

// Create and initialize the static variables
el::Logger* Logging::m_logger = nullptr;
el::Logger* Logging::m_printer = nullptr;
std::string Logging::m_loggerPath = "";
std::string Logging::m_printerPath = "";
std::string Logging::m_runId = "";
int Logging::m_numLoggerFiles = 0;
int Logging::m_numPrinterFiles = 0;

el::Logger* Logging::logger() {
    ASSERT(m_logger != nullptr);
    return m_logger;
}

el::Logger* Logging::printer() {
    ASSERT(m_printer != nullptr);
    return m_printer;
}

void Logging::initialize(const std::string& runId) {

    // Ensure we only initialize the loggers once
    static bool initialized = false;
    ASSERT(!initialized);
    initialized = true;

    // Set the runId
    m_runId = runId;

    // The logger names
    const std::string loggerName = "logger";
    const std::string printerName = "printer";

    // Set the file paths
    m_loggerPath = SimUtilities::getProjectDirectory() + "run/" + m_runId + "/log/last.txt";
    m_printerPath = SimUtilities::getProjectDirectory() + "run/" + m_runId + "/print/last.txt";

    // Register and configure the logger
    el::Configurations logConfig;
    m_logger = el::Loggers::getLogger(loggerName);
    logConfig.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    logConfig.setGlobally(el::ConfigurationType::Filename, m_loggerPath);
    logConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
        std::to_string(10 * 1024 * 1024)); // 10 MiB, ~10,000 lines
    logConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
    logConfig.setGlobally(el::ConfigurationType::Format,
        "%datetime{%Y-%M-%d %H:%m:%s.%g} [%level] %msg");
    el::Loggers::reconfigureLogger(loggerName, logConfig);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Helpers::installPreRollOutCallback(rolloutHandler);

    // Register and configure the printer
    el::Configurations printConfig;
    m_printer = el::Loggers::getLogger(printerName);
    printConfig.setGlobally(el::ConfigurationType::Filename, m_printerPath);
    printConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
        std::to_string(10 * 1024 * 1024)); // 10 MiB, ~10,000 lines
    printConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
    printConfig.setGlobally(el::ConfigurationType::Format, "[%level] - %msg");
    el::Loggers::reconfigureLogger(printerName, printConfig);
}

std::string Logging::getNextFileName(const char* filename) {
    std::string path = "";
    if (std::string(filename) == m_loggerPath) {
        m_numLoggerFiles += 1;
        path = "/log/" + std::to_string(m_numLoggerFiles) + ".txt";
    }
    else if (std::string(filename) == m_printerPath) {
        m_numPrinterFiles += 1;
        path = "/print/" + std::to_string(m_numPrinterFiles) + ".txt";
    }
    return SimUtilities::getProjectDirectory() + "run/" + m_runId + path;
}

void Logging::rolloutHandler(const char* filename, std::size_t size) {
    int value = std::rename(filename, getNextFileName(filename).c_str());
    ASSERT_EQUAL(value, 0);
}

} // namespace sim
