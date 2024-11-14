#include <QStringList>

#include "ProcessUtilities.h"

namespace mms {

bool ProcessUtilities::start(const QString &command, const QString &directory,
                             QProcess *process) {
  QStringList args = QProcess::splitCommand(command);
  QString bin;
  if (!args.isEmpty()) {
    bin = args.at(0);
    args.removeFirst();
  }
  process->setWorkingDirectory(directory);
  process->start(bin, args);
  return process->waitForStarted();
}

}  // namespace mms
