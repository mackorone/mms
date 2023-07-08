#pragma once

#include <QString>

namespace mms {

class SettingsMisc {
 public:
  SettingsMisc() = delete;

  static QString getRecentMazeFile();
  static void setRecentMazeFile(const QString &path);

  static QString getRecentMouseAlgo();
  static void setRecentMouseAlgo(const QString &name);

  static int getRecentWindowWidth();
  static void setRecentWindowWidth(int width);

  static int getRecentWindowHeight();
  static void setRecentWindowHeight(int height);

 private:
  static const QString GROUP;
  static const QString KEY_RECENT_MAZE_ALGO;
  static const QString KEY_RECENT_MOUSE_ALGO;
  static const QString KEY_RECENT_WINDOW_WIDTH;
  static const QString KEY_RECENT_WINDOW_HEIGHT;

  static int getNumber(QString key, int defaultValue);
  static QString getValue(const QString &key);
  static void setValue(const QString &key, const QString &value);
};

}  // namespace mms
