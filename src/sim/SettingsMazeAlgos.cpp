#include "SettingsMazeAlgos.h"

#include "Assert.h"
#include "ConfigDialog.h"
#include "ConfigDialogField.h"
#include "Settings.h"

namespace mms {

const QString SettingsMazeAlgos::GROUP_PREFIX = "mazeAlgos";
const QString SettingsMazeAlgos::NAME_KEY = "name";
const QString SettingsMazeAlgos::DIR_PATH_KEY = "dirPath";
const QString SettingsMazeAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString SettingsMazeAlgos::RUN_COMMAND_KEY = "runCommand";

QStringList SettingsMazeAlgos::names() {
    return Settings::get()->values(GROUP_PREFIX, NAME_KEY);
}

QString SettingsMazeAlgos::getDirPath(const QString& name) {
    return getValue(name, DIR_PATH_KEY);
}

QString SettingsMazeAlgos::getBuildCommand(const QString& name) {
    return getValue(name, BUILD_COMMAND_KEY);
}

QString SettingsMazeAlgos::getRunCommand(const QString& name) {
    return getValue(name, RUN_COMMAND_KEY);
}

QString SettingsMazeAlgos::execImportDialog() {
    return execDialog("");
}

QString SettingsMazeAlgos::execEditDialog(const QString& name) {
    return execDialog(name);
}

QString SettingsMazeAlgos::getValue(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP_PREFIX, NAME_KEY, name);
    ASSERT_EQ(vector.size(), 1);
    return vector.at(0).value(key);
}

QString SettingsMazeAlgos::execDialog(const QString& name) {

    bool isEditDialog = !name.isEmpty();

    ConfigDialogField nameField;
    nameField.key = NAME_KEY;
    nameField.label = "Name";

    ConfigDialogField dirPathField;
    dirPathField.key = DIR_PATH_KEY;
    dirPathField.label = "Directory";
    dirPathField.fileBrowser = true;
    dirPathField.onlyDirectories = true;

    ConfigDialogField buildCommandField;
    buildCommandField.key = BUILD_COMMAND_KEY;
    buildCommandField.label = "Build Command";

    ConfigDialogField runCommandField;
    runCommandField.key = RUN_COMMAND_KEY;
    runCommandField.label = "Run Command";

    // Populate some initial values
    if (isEditDialog) {
        nameField.initialValue = name;
        dirPathField.initialValue = getValue(name, DIR_PATH_KEY);
        buildCommandField.initialValue = getValue(name, BUILD_COMMAND_KEY);
        runCommandField.initialValue = getValue(name, RUN_COMMAND_KEY);
    }

    // Disallow certain names
    QVector<QVariant> nameFilterValues;
    nameFilterValues.append(QVariant(""));
    for (const QString& algoName : names()) {
        if (algoName != name) {
            nameFilterValues.append(QVariant(algoName));
        }
    }
    nameField.filterValues = nameFilterValues;
    nameField.inclusiveFilter = false;

    // Execute the dialog
    QString action = QString((isEditDialog ? "Edit" : "Import"));
    ConfigDialog dialog(
        action,
        "Maze Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField
        },
        isEditDialog
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return QString();
    }

    // Delete was pressed
    if (dialog.deleteButtonPressed()) {
        Settings::get()->remove(GROUP_PREFIX, NAME_KEY, name);
        return QString();
    }

    // Ok was pressed
    QMap<QString, QString> map = {
        {NAME_KEY, dialog.getValue(NAME_KEY)},
        {DIR_PATH_KEY, dialog.getValue(DIR_PATH_KEY)},
        {BUILD_COMMAND_KEY, dialog.getValue(BUILD_COMMAND_KEY)},
        {RUN_COMMAND_KEY, dialog.getValue(RUN_COMMAND_KEY)}
    };
    if (isEditDialog) {
        Settings::get()->update(GROUP_PREFIX, NAME_KEY, name, map);
    }
    else {
        Settings::get()->add(GROUP_PREFIX, map);
    }
    return dialog.getValue(NAME_KEY);
}

} //namespace mms
