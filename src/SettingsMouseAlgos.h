#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class SettingsMouseAlgos {
 public:
  SettingsMouseAlgos() = delete;

  static QStringList names();
  static QString getDirectory(const QString &name);
  static QString getBuildCommand(const QString &name);
  static QString getRunCommand(const QString &name);

  static void add(const QString &name, const QString &directory,
                  const QString &buildCommand, const QString &runCommand);
  static void update(const QString &name, const QString &newName,
                     const QString &newDirectory,
                     const QString &newBuildCommand,
                     const QString &newRunCommand);
  static void remove(const QString &name);

 private:
  static const QString GROUP;
  static const QString KEY_NAME;
  static const QString KEY_DIR_PATH;
  static const QString KEY_BUILD_COMMAND;
  static const QString KEY_RUN_COMMAND;

  static QString getValue(const QString &name, const QString &key);
};

}  // namespace mms
