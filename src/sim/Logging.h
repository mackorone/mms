#pragma once

#include <QDebug>
#include <QString>
#include <QTextStream>

namespace mms {

class Logging {

public:
    Logging() = delete;
    static void init(const QString& runId);

private:
    static QTextStream* STDOUT;
    static QTextStream* STREAM;
    static void handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg);
};

} // namespace mms
