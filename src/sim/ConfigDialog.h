#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>

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
    QString getLineEditValue(const QString& label);

private:

    bool m_removeButtonPressed;
    QDialogButtonBox* m_buttons;
    QVector<ConfigDialogField> m_fields;
    QMap<QString, QLineEdit*> m_lineEdits;

    // Helper functions for append input fields to the layout
    void addLabel(QString text, QGridLayout* layout, int row);
    void appendTextField(ConfigDialogField field, QGridLayout* layout);

    // If invalid, disable the OK button
    void validate();

};

} // namespace mms
