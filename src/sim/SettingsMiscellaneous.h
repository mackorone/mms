#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class SettingsMiscellaneous {

public:

    SettingsMiscellaneous() = delete;
    static void execEditDialog(); // TODO: MACK - shouldn't include window width

    static QString getWindowWidth(); // TODO: MACK - return type...
    static void setWindowWidth();

private:

    static QString getValue(const QString& key);

    // Settings groups and keys
    static const QString GROUP_PREFIX;
    static const QString WINDOW_WIDTH_KEY;

};

} // namespace mms
