#pragma once

#include <QString>
#include <QTextStream>

namespace sim {

class Logging {

public:
    Logging() = delete;
    static void init(const QString& runId);

private:
    static QTextStream* STREAM;
    static void handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg);
};

} // namespace sim
