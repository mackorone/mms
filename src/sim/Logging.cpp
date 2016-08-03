#include "Logging.h"

#include <QDebug>
#include <QDir>
#include <QFile>

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"
#include "Time.h"

namespace sim {

QTextStream* Logging::STREAM = nullptr;

void Logging::init(const QString& runId) {

    SIM_ASSERT_TR(nullptr == STREAM);

    QString dirPath(Directory::get()->getRunDirectory() + runId + "/");
    QDir().mkpath(dirPath);
    QString filePath(dirPath + "log.txt");
    QFile* file = new QFile(filePath);
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    STREAM = new QTextStream(file);

    qInstallMessageHandler(handler);

    // TODO: MACK - auto insert spaces, put quotes
    //QDebug::setAutoInsertSpaces(false); // TODO: MACK
}

void Logging::handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg) {

    // TODO: MACK - benchmark this
    SIM_ASSERT_FA(nullptr == STREAM);
    
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
    *STREAM << formatted << endl << flush; // TODO: MACK - don't need to flush here, I don't think
}

} // namespace sim
