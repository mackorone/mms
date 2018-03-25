#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>

#include "ConfigDialogField.h"

namespace mms {

class ConfigDialog : public QDialog {

    Q_OBJECT

public:

    ConfigDialog(
        const QString& action,
        const QString& object,
        const QVector<ConfigDialogField>& fields,
        bool includeRemoveButton);

    bool removeButtonPressed();
    QString getComboBoxValue(const QString& label);
    QString getLineEditValue(const QString& label);
    bool getComboBoxSelected(const QString& label);

private:

    bool m_removeButtonPressed;
    QDialogButtonBox* m_buttons;
    QVector<ConfigDialogField> m_fields;
    QMap<QString, QComboBox*> m_comboBoxes;
    QMap<QString, QLineEdit*> m_lineEdits;

    // If invalid, disable the OK button
    void validate();

};

} // namespace mms
