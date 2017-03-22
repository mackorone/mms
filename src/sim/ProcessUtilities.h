#pragma once

#include <QProcess>
#include <QString>

namespace mms {

class ProcessUtilities {

public:

    ProcessUtilities() = delete;
    static bool start(QProcess* process, const QString& command);

};

} // namespace mms
