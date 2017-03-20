#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class SettingsMouseAlgos {

public:

    SettingsMouseAlgos() = delete;

    // Accessor functions
    static QStringList names();
    static QString getDirPath(const QString& name);
    static QString getBuildCommand(const QString& name);
    static QString getRunCommand(const QString& name);
    static QString getMouseFilePath(const QString& name);
    
    // Returns the name of the new/updated algo
    static QString execImportDialog();
    static QString execEditDialog(const QString& name);
    
private:

    // Settings groups and keys
    static const QString GROUP_PREFIX;
    static const QString NAME_KEY;
    static const QString DIR_PATH_KEY;
    static const QString BUILD_COMMAND_KEY;
    static const QString RUN_COMMAND_KEY;
    static const QString MOUSE_FILE_PATH_KEY;
    
    // Helper functions
    static QString getValue(const QString& name, const QString& key);
    static QString execDialog(const QString& name);
    
};

} // namespace mms
