#include "Logging.h"

#include <QPair>

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"
#include "Time.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

Logging* Logging::INSTANCE = nullptr;

void Logging::init(const QString& runId) {
    SIM_ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new Logging(runId);
}

el::Logger* Logging::get() {
    SIM_ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE->getLogger("sim"); // TODO: MACK
}

// TODO: MACK
Logging* Logging::getLogging() {
    SIM_ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
}

Logging::Logging(const QString& runId) :
    m_runId(runId),
    m_simLoggerName("sim"),
    m_mazeLoggerName("maze"),
    m_mouseLoggerName("mouse") {

    // For each of the logger names ...
    for (QString loggerName : {m_simLoggerName, m_mazeLoggerName, m_mouseLoggerName}) {
        
        // ... create the logger info ...
        QString loggerPath = Directory::get()->getRunDirectory() + m_runId + "/logs/" + loggerName + "/default.txt";
        m_info.insert(loggerName, {loggerPath, 1});

        // ... and then create the logger ...
        el::Logger* logger = el::Loggers::getLogger(loggerName.toStdString());

        // ... and then configure it
        el::Configurations loggerConfig;
        loggerConfig.setGlobally(el::ConfigurationType::Filename, loggerPath.toStdString());
        loggerConfig.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
        loggerConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
            QString::number(10 * 1024 * 1024).toStdString()); // 10 MiB, ~10,000 lines
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
        double seconds = Time::get()->elapsedRealTime().getSeconds();
        QString secondsString = SimUtilities::formatSeconds(seconds);
        secondsString.truncate(secondsString.indexOf(".") + 4); // Trim to 3 decimal places
        return secondsString.toStdString().c_str();
    }));

    el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%sim_time", [](){
        double seconds = Time::get()->elapsedSimTime().getSeconds();
        QString secondsString = SimUtilities::formatSeconds(seconds);
        secondsString.truncate(secondsString.indexOf(".") + 4); // Trim to 3 decimal places
        return secondsString.toStdString().c_str();
    }));
}

el::Logger* Logging::getLogger(const QString& loggerName) {
    SIM_ASSERT_TR(m_info.contains(loggerName));
    return el::Loggers::getLogger(loggerName.toStdString());
}

QString Logging::getNextFileName(const char* filename) {
    QString path = "";
    for (const auto& pair : ContainerUtilities::items(m_info)) {
        QString loggerName = pair.first;
        QString loggerPath = pair.second.first;
        int numLogFiles = pair.second.second;
        if (QString(filename) == loggerPath) {
            path = "/logs/" + loggerName + "/" + QString::number(numLogFiles) + ".txt";
            m_info[loggerName] = {loggerPath, numLogFiles + 1};
        }
    }
    SIM_ASSERT_NE(path.toStdString().c_str(), "");
    return Directory::get()->getRunDirectory() + m_runId + path;
}

void Logging::rolloutHandler(const char* filename, std::size_t size) {
    int value = std::rename(filename, Logging::getLogging()->getNextFileName(filename).toStdString().c_str());
    SIM_ASSERT_EQ(value, 0);
}

} // namespace sim
