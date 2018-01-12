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
    QString getComboBoxValue(const QString& key);
    QString getLineEditValue(const QString& key);
    bool getComboBoxSelected(const QString& key);

private:

    bool m_removeButtonPressed;
    QDialogButtonBox* m_buttons;
    QMap<QString, QComboBox*> m_comboBoxes;
    QMap<QString, QLineEdit*> m_lineEdits;

};

} // namespace mms
