#include "Logging.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTime>

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"
#include "Time.h"

namespace sim {

QTextStream* Logging::STDOUT = nullptr;
QTextStream* Logging::STREAM = nullptr;

void Logging::init(const QString& runId) {

    SIM_ASSERT_TR(nullptr == STDOUT);
    SIM_ASSERT_TR(nullptr == STREAM);

    STDOUT = new QTextStream(stdout);

    QString dirPath(Directory::get()->getRunDirectory() + runId + "/");
    QDir().mkpath(dirPath);
    QString filePath(dirPath + "log.txt");
    QFile* file = new QFile(filePath);
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    STREAM = new QTextStream(file);

    qInstallMessageHandler(handler);
}

void Logging::handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg) {

    SIM_ASSERT_FA(nullptr == STDOUT);
    SIM_ASSERT_FA(nullptr == STREAM);
    
    static const QMap<QtMsgType, QString> mapping {
        {QtDebugMsg,    "DEBUG"   },
        {QtInfoMsg,     "INFO"    },
        {QtWarningMsg,  "WARN"    },
        {QtCriticalMsg, "CRITICAL"},
        {QtFatalMsg,    "FATAL"   },
        {QtSystemMsg,   "SYSTEM"  },
    };

    static const QString formatString = "mm:ss.zzz";

    int realMs = static_cast<int>(
        Time::get()->elapsedRealTime().getMilliseconds());
    int simMs = static_cast<int>(
        Time::get()->elapsedSimTime().getMilliseconds());

    QString formatted = QString("[ %1 | %2 | %3 ] - %4").arg(
        QTime(0, 0, 0).addMSecs(realMs).toString(formatString),
        QTime(0, 0, 0).addMSecs(simMs).toString(formatString),
        mapping.value(type),
        msg
    );

    *STDOUT << formatted << endl;
    *STREAM << formatted << endl;
}

} // namespace sim
