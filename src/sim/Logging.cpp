#include "Logging.h"

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"
#include "Time.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

// Create and initialize the static variables
std::string Logging::m_runId = "";
std::string Logging::m_simLoggerName = "sim";
std::string Logging::m_mazeLoggerName = "maze";
std::string Logging::m_mouseLoggerName = "mouse";
QMap<std::string, std::pair<std::string, int>> Logging::m_info;

el::Logger* L() {
    return Logging::getSimLogger();
}

el::Logger* Logging::getSimLogger() {
    return getLogger(m_simLoggerName);
}

el::Logger* Logging::getMazeLogger() {
    return getLogger(m_mazeLoggerName);
}

el::Logger* Logging::getMouseLogger() {
    return getLogger(m_mouseLoggerName);
}

void Logging::initialize(const std::string& runId) {

    // Ensure we only initialize the loggers once
    static bool initialized = false;
    SIM_ASSERT_FA(initialized);
    initialized = true;

    // Set the runId
    m_runId = runId;

    // For each of the logger names ...
    for (std::string loggerName : {m_simLoggerName, m_mazeLoggerName, m_mouseLoggerName}) {
        
        // ... create the logger info ...
        std::string loggerPath = Directory::getRunDirectory() + m_runId + "/logs/" + loggerName + "/default.txt";
        m_info.insert(loggerName, std::make_pair(loggerPath, 1));

        // ... and then create the logger ...
        el::Logger* logger = el::Loggers::getLogger(loggerName);

        // ... and then configure it
        el::Configurations loggerConfig;
        loggerConfig.setGlobally(el::ConfigurationType::Filename, loggerPath);
        loggerConfig.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
        loggerConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
            std::to_string(10 * 1024 * 1024)); // 10 MiB, ~10,000 lines
        loggerConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
        loggerConfig.setGlobally(el::ConfigurationType::Format,
            "[ %real_time | %sim_time | %level | %logger ] - %msg");
        el::Loggers::reconfigureLogger(logger, loggerConfig);
    }

    // Set some flags and the rollout callback
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Helpers::installPreRollOutCallback(rolloutHandler);

    el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%real_time", [](){
        double seconds = T()->elapsedRealTime().getSeconds();
        std::string secondsString = SimUtilities::formatSeconds(seconds);
        return secondsString.substr(0, secondsString.find(".") + 4).c_str(); // Trim to 3 decimal places
    }));

    el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%sim_time", [](){
        double seconds = T()->elapsedSimTime().getSeconds();
        std::string secondsString = SimUtilities::formatSeconds(seconds);
        return secondsString.substr(0, secondsString.find(".") + 4).c_str(); // Trim to 3 decimal places
    }));
}

el::Logger* Logging::getLogger(const std::string& loggerName) {
    SIM_ASSERT_TR(m_info.contains(loggerName.c_str()));
    return el::Loggers::getLogger(loggerName);
}

std::string Logging::getNextFileName(const char* filename) {
    std::string path = "";
    for (const auto& pair : ContainerUtilities::items(m_info)) {
        std::string loggerName = pair.first;
        std::string loggerPath = pair.second.first;
        int numLogFiles = pair.second.second;
        if (std::string(filename) == loggerPath) {
            path = "/logs/" + loggerName + "/" + std::to_string(numLogFiles) + ".txt";
            m_info[loggerName.c_str()] = std::make_pair(loggerPath, numLogFiles + 1);
        }
    }
    SIM_ASSERT_NE(path, "");
    return Directory::getRunDirectory() + m_runId + path;
}

void Logging::rolloutHandler(const char* filename, std::size_t size) {
    int value = std::rename(filename, getNextFileName(filename).c_str());
    SIM_ASSERT_EQ(value, 0);
}

} // namespace sim
