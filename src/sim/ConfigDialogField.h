#pragma once

#include <QString>
#include <QVariant>
#include <QVector>

namespace mms {

enum class ConfigDialogFieldType {
    BOOLEAN,
    DIRECTORY,
    FILE_,
    FLOAT,
    INTEGER,
    STRING,
};

struct ConfigDialogField {

    QString label;
    ConfigDialogFieldType type;

    QVector<QVariant> comboBoxValues;
    QVariant initialComboBoxValue;
    QVariant initialLineEditValue;
    bool allowEmptyLineEditValue = true;
    bool comboBoxSelected = true;

    bool fileBrowser = false;
    bool onlyDirectories = false;
}; 

} // namespace mms
