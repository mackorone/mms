#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QStringList>

namespace mms {

class CommandDialog : public QDialog {

    Q_OBJECT

public:

    CommandDialog(const QStringList& names, QWidget* parent);
    CommandDialog(
        const QStringList& names,
        const QString& name,
        const QString& directory,
        const QString& buildCommand,
        const QString& runCommand,
        QWidget* parent);

    QString getName();
    QString getDirectory();
    QString getBuildCommand();
    QString getRunCommand();

private:

    QStringList m_names;
    QLineEdit* m_nameEdit;

    QLineEdit* m_directoryEdit;
    QLineEdit* m_buildCommandEdit;
    QLineEdit* m_runCommandEdit;
    QDialogButtonBox* m_buttons;

    void browse();
    void nameTextChanged();

};

} // namespace mms
