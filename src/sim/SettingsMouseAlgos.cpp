#include "SettingsMouseAlgos.h"

#include "Assert.h"
#include "ConfigDialog.h"
#include "ConfigDialogField.h"
#include "Settings.h"

namespace mms {

const QString SettingsMouseAlgos::GROUP_PREFIX = "mouseAlgos";
const QString SettingsMouseAlgos::NAME_KEY = "name";
const QString SettingsMouseAlgos::DIR_PATH_KEY = "dirPath";
const QString SettingsMouseAlgos::BUILD_COMMAND_KEY = "buildCommand";
const QString SettingsMouseAlgos::RUN_COMMAND_KEY = "runCommand";
const QString SettingsMouseAlgos::MOUSE_FILE_PATH_KEY = "mouseFilePath";

QStringList SettingsMouseAlgos::names() {
    return Settings::get()->values(GROUP_PREFIX, NAME_KEY);
}

QString SettingsMouseAlgos::getDirPath(const QString& name) {
    return getValue(name, DIR_PATH_KEY);
}

QString SettingsMouseAlgos::getBuildCommand(const QString& name) {
    return getValue(name, BUILD_COMMAND_KEY);
}

QString SettingsMouseAlgos::getRunCommand(const QString& name) {
    return getValue(name, RUN_COMMAND_KEY);
}

QString SettingsMouseAlgos::getMouseFilePath(const QString& name) {
    return getValue(name, MOUSE_FILE_PATH_KEY);
}

QString SettingsMouseAlgos::execImportDialog() {
    return execDialog("");
}

QString SettingsMouseAlgos::execEditDialog(const QString& name) {
    return execDialog(name);
}

QString SettingsMouseAlgos::getValue(const QString& name, const QString& key) {
    const auto& vector = Settings::get()->find(GROUP_PREFIX, NAME_KEY, name);
    ASSERT_EQ(vector.size(), 1); 
    return vector.at(0).value(key);
}

QString SettingsMouseAlgos::execDialog(const QString& name) {

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

    ConfigDialogField mouseFilePathField;
    mouseFilePathField.key = MOUSE_FILE_PATH_KEY;
    mouseFilePathField.label = "Mouse File";
    mouseFilePathField.fileBrowser = true;

    // Populate some initial values
    if (isEditDialog) {
        nameField.initialValue = name;
        dirPathField.initialValue = getValue(name, DIR_PATH_KEY);
        buildCommandField.initialValue = getValue(name, BUILD_COMMAND_KEY);
        runCommandField.initialValue = getValue(name, RUN_COMMAND_KEY);
        mouseFilePathField.initialValue = getValue(name, MOUSE_FILE_PATH_KEY);
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
        "Mouse Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField,
            mouseFilePathField
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
        {RUN_COMMAND_KEY, dialog.getValue(RUN_COMMAND_KEY)},
        {MOUSE_FILE_PATH_KEY, dialog.getValue(MOUSE_FILE_PATH_KEY)}
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
