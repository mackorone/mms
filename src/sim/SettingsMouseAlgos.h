#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class SettingsMouseAlgos {

public:

    SettingsMouseAlgos() = delete;

    static QStringList names();
    static QString getDirPath(const QString& name);
    static QString getBuildCommand(const QString& name);
    static QString getRunCommand(const QString& name);
    static QString getMouseFilePathComboBoxValue(const QString& name);
    static QString getMouseFilePathLineEditValue(const QString& name);
    static bool getMouseFilePathComboBoxSelected(const QString& name);

    static void add(
        const QString& name,
        const QString& dirPath,
        const QString& buildCommand,
        const QString& runCommand,
        const QString& mouseFilePathComboBoxValue,
        const QString& mouseFilePathLineEditValue,
        bool mouseFilePathComboBoxSelected
    );
    static void update(
        const QString& name,
        const QString& newName,
        const QString& newDirPath,
        const QString& newBuildCommand,
        const QString& newRunCommand,
        const QString& newMouseFilePathComboBoxValue,
        const QString& newMouseFilePathLineEditValue,
        bool newMouseFilePathComboBoxSelected
    );
    static void remove(const QString& name);

private:

    static const QString GROUP_PREFIX;
    static const QString NAME_KEY;
    static const QString DIR_PATH_KEY;
    static const QString BUILD_COMMAND_KEY;
    static const QString RUN_COMMAND_KEY;
    static const QString MOUSE_FILE_PATH_COMBO_BOX_VALUE_KEY;
    static const QString MOUSE_FILE_PATH_LINE_EDIT_VALUE_KEY;
    static const QString MOUSE_FILE_PATH_COMBO_BOX_SELECTED_KEY;
    
    static QString getValue(const QString& name, const QString& key);
    
};

} // namespace mms
