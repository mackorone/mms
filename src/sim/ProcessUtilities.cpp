#include "ProcessUtilities.h"

#include <QStringList>

namespace mms {

void ProcessUtilities::start(QProcess* process, const QString& command) {
    QStringList args = command.split(' ', QString::SkipEmptyParts);
    if (args.isEmpty()) {
        // TODO: upforgrabs
        // Indicate to the calling function that no command was started
        return;
    }
    QString bin = args.at(0);
    args.removeFirst();
    process->start(bin, args);
}

} // namespace mms
