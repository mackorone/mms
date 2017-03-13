#pragma once

#include <QProcess>
#include <QString>

namespace mms {

class ProcessUtilities {

public:

    ProcessUtilities() = delete;
    static void start(QProcess* process, const QString& command);

};

} // namespace mms
