#include "CommandDialog.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace mms {

CommandDialog::CommandDialog(const QStringList& names, QWidget* parent) :
        CommandDialog(names, "", "", "", "", parent) {
}

CommandDialog::CommandDialog(
        const QStringList& names,
        const QString& name,
        const QString& directory,
        const QString& buildCommand,
        const QString& runCommand,
        QWidget* parent) :
        QDialog(parent),
        m_names(names),
        m_nameEdit(nullptr),
        m_directoryEdit(nullptr),
        m_buildCommandEdit(nullptr),
        m_runCommandEdit(nullptr),
        m_buttons(nullptr) {

    // Set the main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    // Add a groupbox
    QGroupBox* groupBox = new QGroupBox();
    QGridLayout* gridLayout = new QGridLayout();
    groupBox->setLayout(gridLayout);
    int row = 0;

    // Add name
    QLabel* nameLabel = new QLabel("Name:");
    nameLabel->setAlignment(Qt::AlignRight);
    m_nameEdit = new QLineEdit(name);
    gridLayout->addWidget(nameLabel, row, 0);
    gridLayout->addWidget(m_nameEdit, row, 2);
    connect(m_nameEdit, &QLineEdit::textChanged, this, &CommandDialog::nameTextChanged);
    row += 1;
    
    // Add directory
    QLabel* directoryLabel = new QLabel("Directory:");
    directoryLabel->setAlignment(Qt::AlignRight);
    m_directoryEdit = new QLineEdit(directory);
    QPushButton* browseButton = new QPushButton("Browse");
    gridLayout->addWidget(directoryLabel, row, 0);
    gridLayout->addWidget(m_directoryEdit, row, 2);
    gridLayout->addWidget(browseButton, row, 3);
    connect(browseButton, &QPushButton::clicked, this, &CommandDialog::browse);
    row += 1;
    
    // Add build command
    QLabel* buildLabel = new QLabel("Build Command:");
    buildLabel->setAlignment(Qt::AlignRight);
    m_buildCommandEdit = new QLineEdit(buildCommand);
    gridLayout->addWidget(buildLabel, row, 0);
    gridLayout->addWidget(m_buildCommandEdit, row, 2);
    row += 1;

    // Add run command
    m_runCommandEdit = new QLineEdit(runCommand);
    QLabel* runLabel = new QLabel("Run Command:");
    runLabel->setAlignment(Qt::AlignRight);
    gridLayout->addWidget(runLabel, row, 0);
    gridLayout->addWidget(m_runCommandEdit, row, 2);
    row += 1;

    // Add OK and cancel buttons
    m_buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Add group box and buttons to dialog
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(m_buttons);

    // Misc. initialization
    resize(800, height());
    setWindowTitle("New Maze Algorithm");
    nameTextChanged();
}

QString CommandDialog::getName() {
    return m_nameEdit->text();
}

QString CommandDialog::getDirectory() {
    return m_directoryEdit->text();
}

QString CommandDialog::getBuildCommand() {
    return m_buildCommandEdit->text();
}

QString CommandDialog::getRunCommand() {
    return m_runCommandEdit->text();
}

void CommandDialog::browse() {
    QString directory = QDir::toNativeSeparators(
        QFileDialog::getExistingDirectory(
            this,
            tr("Choose Directory"),
            QDir::currentPath()
        )
    );
    if (!directory.isNull()) {
        m_directoryEdit->setText(directory);
    }
}

void CommandDialog::nameTextChanged() {
    QString name = m_nameEdit->text();
    QPushButton* okButton = m_buttons->button(QDialogButtonBox::Ok);
    if (name.isEmpty()) {
        okButton->setEnabled(false);
    }
    else if (m_names.contains(name)) {
        okButton->setEnabled(false);
    }
    else {
        okButton->setEnabled(true);
    }
}

} // namespace mms
