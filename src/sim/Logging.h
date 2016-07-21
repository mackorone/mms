#pragma once

#include <QFile>
#include <QString>

namespace sim {

class Logging {

public:

    // Should only be called once, at start time
    static void init(const QString& runId);

private:

    // A private constructor is used to ensure
    // only one instance of this class exists
    Logging(const QString& runId);

    static Logging* INSTANCE;

    // The file that we'll log to
    QFile m_logFile;

    // Gets called for every log statement
    // TODO: MACK Just use a lambda
    static void handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg);

};

} // namespace sim
