#pragma once

#include <QDebug>
#include <QString>
#include <QTextStream>

namespace mms {

class Logging {
 public:
  Logging() = delete;
  static void init();

 private:
  static QTextStream *STDOUT;
  static void handler(QtMsgType type, const QMessageLogContext &context,
                      const QString &msg);
};

}  // namespace mms
