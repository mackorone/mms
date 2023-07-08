#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>

namespace mms {

class ConfigDialog : public QDialog {
  Q_OBJECT

 public:
  ConfigDialog(QString name, QString directory, QString buildCommand,
               QString runCommand);

  QString getName();
  QString getDirectory();
  QString getBuildCommand();
  QString getRunCommand();
  bool removeButtonPressed();

 private:
  QLineEdit *m_name;
  QLineEdit *m_directory;
  QLineEdit *m_buildCommand;
  QLineEdit *m_runCommand;
  bool m_removeButtonPressed;
  QDialogButtonBox *m_buttons;

  void onBrowseButtonPressed();
  void onRemoveButtonPressed();
  void validate(QString name);

  void appendRow(QGridLayout *layout, QString label, QLineEdit *lineEdit);
};

}  // namespace mms
