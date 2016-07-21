#include "Logging.h"

#include <QTextStream>

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"
#include "Time.h"

namespace sim {

Logging* Logging::INSTANCE = nullptr;

void Logging::init(const QString& runId) {
    SIM_ASSERT_TR(nullptr == INSTANCE);
    INSTANCE = new Logging(runId);
}

Logging::Logging(const QString& runId) {

    // TODO: See http://doc.qt.io/qt-5/qloggingcategory.html#setFilterRules
    // TODO: http://doc.qt.io/qt-5/qtglobal.html#qSetMessagePattern
    qInstallMessageHandler(handler);

    // ... create the logger info ...
    /*
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
    */
}

void Logging::handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg) {

    SIM_ASSERT_FA(nullptr == INSTANCE);
    // TODO: MACK - benchmark this
    
    // TODO: MACK - if debug, we want line numbers
    // "[ %real_time | %sim_time | %level | %logger ] - %msg");
    static const QMap<QtMsgType, QString> mapping {
        {QtDebugMsg,    "DEBUG"   },
        {QtInfoMsg,     "INFO"    },
        {QtWarningMsg,  "WARN"    },
        {QtCriticalMsg, "CRITICAL"},
        {QtFatalMsg,    "FATAL"   },
        {QtSystemMsg,   "SYSTEM"  },
    };

    // TODO: MACK - why is this 9 sometimes?
    double seconds = Time::get()->elapsedRealTime().getSeconds();
    QString secondsString = SimUtilities::formatSeconds(seconds);
    secondsString.truncate(secondsString.indexOf(".") + 4); // Trim to 3 decimal places

    double sim_seconds = Time::get()->elapsedSimTime().getSeconds();
    QString sim_secondsString = SimUtilities::formatSeconds(sim_seconds);
    sim_secondsString.truncate(sim_secondsString.indexOf(".") + 4); // Trim to 3 decimal places

    QString formatted = QString("[ %1 | %2 | %3 ] - %4").arg(
        secondsString,
        sim_secondsString,
        mapping.value(type),
        msg
    );

    std::cout << formatted.toStdString() << std::endl;

    QFile outFile("/home/mack/Desktop/test-log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << formatted << endl << flush;
}

} // namespace sim
