#include "SettingsMiscellaneous.h"

#include "Assert.h"
#include "ConfigDialog.h"
#include "ConfigDialogField.h"
#include "Settings.h"

namespace mms {

const QString SettingsMiscellaneous::GROUP_PREFIX = "miscellaneous";
const QString SettingsMiscellaneous::WINDOW_WIDTH_KEY = "windowWidth";

void SettingsMiscellaneous::execEditDialog() {

    ConfigDialogField windowWidthField;
    windowWidthField.key = WINDOW_WIDTH_KEY;
    windowWidthField.label = "Window Width";
    windowWidthField.initialValue = getValue(WINDOW_WIDTH_KEY);

    // Execute the dialog
    ConfigDialog dialog(
        "Edit",
        "Miscellaneous",
        {
            windowWidthField,
        },
        false
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Ok was pressed
    for (const QString& key : {
        WINDOW_WIDTH_KEY,
    }) {
        Settings::get()->update(GROUP_PREFIX, key, dialog.getValue(key));
    }
}

QString SettingsMiscellaneous::getWindowWidth() {
    return getValue(WINDOW_WIDTH_KEY);
}

QString SettingsMiscellaneous::getValue(const QString& key) {
    return Settings::get()->value(GROUP_PREFIX, key);
}

} //namespace mms
