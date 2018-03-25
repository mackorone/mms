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

    static const QString GROUP;
    static const QString KEY_NAME;
    static const QString KEY_DIR_PATH;
    static const QString KEY_BUILD_COMMAND;
    static const QString KEY_RUN_COMMAND;
    static const QString KEY_MOUSE_FILE_PATH_COMBO_BOX_VALUE;
    static const QString KEY_MOUSE_FILE_PATH_LINE_EDIT_VALUE;
    static const QString KEY_MOUSE_FILE_PATH_COMBO_BOX_SELECTED;
    
    static QString getValue(const QString& name, const QString& key);
    
};

} // namespace mms
