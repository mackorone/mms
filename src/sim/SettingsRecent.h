#pragma once

#include <QString>

namespace mms {

class SettingsRecent {

public:

    SettingsRecent() = delete;

    static QString getRecentMazeAlgo();
    static QString getRecentMouseAlgo();
    static void setRecentMazeAlgo(const QString& name);
    static void setRecentMouseAlgo(const QString& name);

private:

    static const QString GROUP;
    static const QString KEY_MAZE_ALGO;
    static const QString KEY_MOUSE_ALGO;

    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);

};

} // namespace mms
