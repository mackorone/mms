#include "ProcessUtilities.h"

#include <QStringList>

namespace mms {

bool ProcessUtilities::start(
    const QString& command,
    const QString& directory,
    QProcess* process
) {
    QStringList args = command.split(' ', QString::SkipEmptyParts);
    QString bin;
    if (!args.isEmpty()) {
        bin = args.at(0);
        args.removeFirst();
    }
    process->setWorkingDirectory(directory);
    process->start(bin, args);
    return process->waitForStarted();
}

} 
