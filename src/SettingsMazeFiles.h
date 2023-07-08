#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class SettingsMazeFiles {
 public:
  SettingsMazeFiles() = delete;
  static QStringList getAllPaths();
  static void addPath(QString path);
  static void removePath(QString path);

 private:
  static const QString GROUP;
  static const QString KEY_PATH;
};

}  // namespace mms
