#pragma once

#include <QProcess>
#include <QString>

namespace mms {

class ProcessUtilities {
 public:
  ProcessUtilities() = delete;

  static bool start(const QString &command, const QString &directory,
                    QProcess *process);
};

}  // namespace mms
