#pragma once

#include <QString>
#include <QVariant>
#include <QVector>

namespace mms {

struct ConfigDialogField {
    QString key;
    QString label;
    QVector<QVariant> comboBoxValues;
    QVariant initialComboBoxValue;
    QVariant initialLineEditValue;
    bool allowEmptyLineEditValue = true;
    bool comboBoxSelected = true;
    bool fileBrowser = false;
    bool onlyDirectories = false;
}; 

} // namespace mms
