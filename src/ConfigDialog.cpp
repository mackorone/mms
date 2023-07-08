#include "ConfigDialog.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace mms {

ConfigDialog::ConfigDialog(QString name, QString directory,
                           QString buildCommand, QString runCommand)
    : m_name(new QLineEdit(name)),
      m_directory(new QLineEdit(directory)),
      m_buildCommand(new QLineEdit(buildCommand)),
      m_runCommand(new QLineEdit(runCommand)),
      m_removeButtonPressed(false) {
  // Set the layout for the dialog
  QVBoxLayout *mainLayout = new QVBoxLayout();
  setLayout(mainLayout);

  // Add a groupbox
  QGroupBox *groupBox = new QGroupBox();
  QGridLayout *gridLayout = new QGridLayout();
  groupBox->setLayout(gridLayout);

  // Add a row for each field
  appendRow(gridLayout, "Name", m_name);
  appendRow(gridLayout, "Directory", m_directory);
  appendRow(gridLayout, "Build Command", m_buildCommand);
  appendRow(gridLayout, "Run Command", m_runCommand);

  // Enforce nonempty name field
  connect(m_name, &QLineEdit::textChanged, this, &ConfigDialog::validate);

  // Add OK and cancel buttons
  m_buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  // Potentially add a remove button
  bool isEditing = !m_name->text().isEmpty();
  if (isEditing) {
    QPushButton *removeButton =
        m_buttons->addButton("Remove", QDialogButtonBox::DestructiveRole);
    connect(removeButton, &QPushButton::clicked, this,
            &ConfigDialog::onRemoveButtonPressed);
  }

  // Add the group box and buttons to dialog
  mainLayout->addWidget(groupBox);
  mainLayout->addWidget(m_buttons);

  // Misc initialization
  resize(800, 0);
  QString action = isEditing ? "Edit" : "New";
  setWindowTitle(action + " Mouse Algorithm");
  validate(name);
}

QString ConfigDialog::getName() { return m_name->text(); }

QString ConfigDialog::getDirectory() { return m_directory->text(); }

QString ConfigDialog::getBuildCommand() { return m_buildCommand->text(); }

QString ConfigDialog::getRunCommand() { return m_runCommand->text(); }

bool ConfigDialog::removeButtonPressed() { return m_removeButtonPressed; }

void ConfigDialog::onBrowseButtonPressed() {
  QString path;
  path = QFileDialog::getExistingDirectory(
      this, tr("Choose Directory"), QString(), QFileDialog::ShowDirsOnly);
  if (!path.isNull()) {
    m_directory->setText(QDir::toNativeSeparators(path));
  }
}

void ConfigDialog::onRemoveButtonPressed() {
  QMessageBox::StandardButton response =
      QMessageBox::question(this, "Remove Mouse Algorithm", "Are you sure?");
  if (response == QMessageBox::StandardButton::Yes) {
    m_removeButtonPressed = true;
    accept();
  }
}

void ConfigDialog::validate(QString name) {
  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(!name.isEmpty());
}

void ConfigDialog::appendRow(QGridLayout *layout, QString label,
                             QLineEdit *lineEdit) {
  // Create the label
  int row = layout->rowCount();
  QLabel *rowLabel = new QLabel(label);
  rowLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  // Create browse button (only for directory)
  QPushButton *browseButton = nullptr;
  if (lineEdit == m_directory) {
    browseButton = new QPushButton("Browse");
    connect(browseButton, &QPushButton::clicked, this,
            &ConfigDialog::onBrowseButtonPressed);
  }

  // Add the widgets to the layout
  layout->addWidget(rowLabel, row, 0);
  layout->addWidget(lineEdit, row, 1);
  if (browseButton != nullptr) {
    layout->addWidget(browseButton, row, 2);
  }
}

}  // namespace mms
