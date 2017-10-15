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

    static const QString GROUP_PREFIX;
    static const QString MAZE_ALGO_KEY;
    static const QString MOUSE_ALGO_KEY;

    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);

};

} // namespace mms
