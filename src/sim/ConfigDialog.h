#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpinBox>

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

    // TODO: MACK
    QString getComboBoxValue(const QString& label);
    QString getLineEditValue(const QString& label);
    bool getComboBoxSelected(const QString& label);

private:

    bool m_removeButtonPressed;
    QDialogButtonBox* m_buttons;
    QVector<ConfigDialogField> m_fields;

    // Inputs for different types of fields
    QMap<QString, QComboBox*> m_comboBoxes;
    QMap<QString, QLineEdit*> m_lineEdits;
    QMap<QString, QSpinBox*> m_spinBoxes;
    QMap<QString, QDoubleSpinBox*> m_doubleSpinBoxes;

    // Helper functions for append input fields to the layout
    void addLabel(QString text, QGridLayout* layout, int row);
    void appendNumberField(ConfigDialogField field, QGridLayout* layout);
    void appendTextField(ConfigDialogField field, QGridLayout* layout);

    // If invalid, disable the OK button
    void validate();

};

} // namespace mms
