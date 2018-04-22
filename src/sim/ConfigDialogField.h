#pragma once

#include <QString>
#include <QVariant>
#include <QVector>

namespace mms {

enum class ConfigDialogFieldType {
    DIRECTORY,
    FILE,
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
    QVariant initialSpinBoxValue;
    QVariant initialDoubleSpinBoxValue;

    bool allowEmptyLineEditValue = true;
    bool comboBoxSelected = true;
}; 

} // namespace mms
