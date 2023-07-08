#include "Logging.h"

#include "AssertMacros.h"

namespace mms {

QTextStream *Logging::STDOUT = nullptr;

void Logging::init() {
  ASSERT_TR(STDOUT == nullptr);
  STDOUT = new QTextStream(stdout);
  qInstallMessageHandler(handler);
}

void Logging::handler(QtMsgType type, const QMessageLogContext &context,
                      const QString &msg) {
  ASSERT_FA(STDOUT == nullptr);

  QString formatted =
      QString("[%1:%2] - %3")
          .arg(context.file, QString::number(context.line), msg);

  *STDOUT << formatted << Qt::endl;
}

}  // namespace mms
