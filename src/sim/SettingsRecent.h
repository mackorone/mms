#pragma once

#include <QString>

namespace mms {

class SettingsRecent {

public:

    SettingsRecent() = delete;

    static QString getRecentMazeAlgo();
    static void setRecentMazeAlgo(const QString& name);

    static QString getRecentMouseAlgo();
    static void setRecentMouseAlgo(const QString& name);

    static int getRecentWindowWidth();
    static void setRecentWindowWidth(int width);

    static int getRecentWindowHeight();
    static void setRecentWindowHeight(int height);

private:

    static const QString GROUP;
    static const QString KEY_MAZE_ALGO;
    static const QString KEY_MOUSE_ALGO;
    static const QString KEY_WINDOW_WIDTH;
    static const QString KEY_WINDOW_HEIGHT;

    static int getNumber(QString key, int defaultValue);
    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);

};

} // namespace mms
