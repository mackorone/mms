#include "Logging.h"

#include "Assert.h"

namespace mms {

QTextStream* Logging::STDOUT = nullptr;

void Logging::init() {
    ASSERT_TR(STDOUT == nullptr);
    STDOUT = new QTextStream(stdout);
    qInstallMessageHandler(handler);
}

void Logging::handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg) {

    ASSERT_FA(STDOUT == nullptr);
    
    static const QMap<QtMsgType, QString> mapping {
        {QtDebugMsg,    "DEBUG"   },
        {QtInfoMsg,     "INFO"    },
        {QtWarningMsg,  "WARN"    },
        {QtCriticalMsg, "CRITICAL"},
        {QtFatalMsg,    "FATAL"   },
    };

    QString formatted = QString("[%1][%2:%3] - %4").arg(
        mapping.value(type),
        context.file,
        QString::number(context.line),
        msg
    );

    *STDOUT << formatted << endl;
}

} // namespace mms
