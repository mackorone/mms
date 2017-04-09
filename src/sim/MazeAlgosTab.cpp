#include "MazeAlgosTab.h"

#include <QDateTime>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QTabWidget>
#include <QVBoxLayout>

#include "ConfigDialog.h"
#include "ProcessUtilities.h"
#include "SettingsMazeAlgos.h"
#include "SimUtilities.h"

namespace mms {

MazeAlgosTab::MazeAlgosTab() :
        m_comboBox(new QComboBox()),
        m_editButton(new QPushButton("Edit")),
        m_widthBox(new QSpinBox()),
        m_heightBox(new QSpinBox()),
        m_prevSeedBox(new QSpinBox()),
        m_nextSeedBox(new QSpinBox()),
        m_buildButton(new QPushButton("Build")),
        m_buildDisplay(new TextDisplayWidget()),
        m_runButton(new QPushButton("Run")),
        m_runDisplay(new TextDisplayWidget()) {

    // Set up the layout
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    QHBoxLayout* topLayout = new QHBoxLayout();
    layout->addLayout(topLayout);

    // Add the import button
    QPushButton* importButton = new QPushButton("Import Maze Algorithm");
    connect(importButton, &QPushButton::clicked, this, &MazeAlgosTab::import);
    topLayout->addWidget(importButton);

    // Add the combobox
    topLayout->addWidget(m_comboBox);

    // Create the algo buttons
    connect(m_editButton, &QPushButton::clicked, this, &MazeAlgosTab::edit);
    connect(m_buildButton, &QPushButton::clicked, this, &MazeAlgosTab::build);
    connect(m_runButton, &QPushButton::clicked, this, &MazeAlgosTab::run);

    // Add the algo buttons to the tab
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_runButton);
    buttonsLayout->addWidget(m_buildButton);
    buttonsLayout->addWidget(m_editButton);
    layout->addLayout(buttonsLayout);

    // Add runtime options
    QHBoxLayout* optionsLayout = new QHBoxLayout();
    layout->addLayout(optionsLayout);

    // Add the maze size box
    QGroupBox* mazeSizeBox = new QGroupBox("Maze Size");
    optionsLayout->addWidget(mazeSizeBox);
    QGridLayout* mazeSizeLayout = new QGridLayout();
    mazeSizeBox->setLayout(mazeSizeLayout);

    // Add the maze size inputs
    m_widthBox->setValue(16);
    m_heightBox->setValue(16);
    QLabel* widthLabel = new QLabel("Width");
    QLabel* heightLabel = new QLabel("Height");
    widthLabel->setAlignment(Qt::AlignCenter);
    heightLabel->setAlignment(Qt::AlignCenter);
    mazeSizeLayout->addWidget(widthLabel, 0, 0);
    mazeSizeLayout->addWidget(m_widthBox, 0, 1);
    mazeSizeLayout->addWidget(heightLabel, 0, 2);
    mazeSizeLayout->addWidget(m_heightBox, 0, 3);

    // Add the random seed box
    QGroupBox* randomSeedBox = new QGroupBox("Random Seed");
    optionsLayout->addWidget(randomSeedBox);
    QGridLayout* randomSeedLayout = new QGridLayout();
    randomSeedBox->setLayout(randomSeedLayout);

    // Add the random seed inputs
    m_prevSeedBox->setReadOnly(true);
    m_prevSeedBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_prevSeedBox->setRange(0, MAX_SEED_VALUE);
    m_nextSeedBox->setRange(0, MAX_SEED_VALUE);
	m_nextSeedBox->setValue(getNextSeed());
    QLabel* prevLabel = new QLabel("Previous");
    QLabel* nextLabel = new QLabel("Next");
    nextLabel->setAlignment(Qt::AlignCenter);
    prevLabel->setAlignment(Qt::AlignCenter);
    randomSeedLayout->addWidget(prevLabel, 0, 0);
    randomSeedLayout->addWidget(m_prevSeedBox, 0, 1);
    randomSeedLayout->addWidget(nextLabel, 0, 2);
    randomSeedLayout->addWidget(m_nextSeedBox, 0, 3);

    // Add the build and run output
    QTabWidget* tabWidget = new QTabWidget();
    layout->addWidget(tabWidget);
	tabWidget->addTab(m_buildDisplay, "Build Output");
	tabWidget->addTab(m_runDisplay, "Run Output");
    connect(m_buildButton, &QPushButton::clicked, this, [=](){
        tabWidget->setCurrentWidget(m_buildDisplay);
    });
    connect(m_runButton, &QPushButton::clicked, this, [=](){
        tabWidget->setCurrentWidget(m_runDisplay);
    });

    // Add the maze algos
    refresh();
}

void MazeAlgosTab::import() {

    QVector<ConfigDialogField> fields = getFields();
    ConfigDialogField nameField = fields.at(0);
    ConfigDialogField dirPathField = fields.at(1);
    ConfigDialogField buildCommandField = fields.at(2);
    ConfigDialogField runCommandField = fields.at(3);

    ConfigDialog dialog(
        "Import",
        "Maze Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField
        },
        false // No "Remove" button
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Ok was pressed
    QString name = dialog.getValue(nameField.key);
    SettingsMazeAlgos::add(
        name,
        dialog.getValue(dirPathField.key),
        dialog.getValue(buildCommandField.key),
        dialog.getValue(runCommandField.key)
    );

    // Update the maze algos
    refresh(name);
}

void MazeAlgosTab::edit() {

    QString name = m_comboBox->currentText();

    QVector<ConfigDialogField> fields = getFields();
    ConfigDialogField nameField = fields.at(0);
    ConfigDialogField dirPathField = fields.at(1);
    ConfigDialogField buildCommandField = fields.at(2);
    ConfigDialogField runCommandField = fields.at(3);

    nameField.initialValue = name;
    dirPathField.initialValue = SettingsMazeAlgos::getDirPath(name);
    buildCommandField.initialValue = SettingsMazeAlgos::getBuildCommand(name);
    runCommandField.initialValue = SettingsMazeAlgos::getRunCommand(name);

    ConfigDialog dialog(
        "Edit",
        "Maze Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField
        },
        true // Include a "Remove" button
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Remove was pressed
    if (dialog.removeButtonPressed()) {
        SettingsMazeAlgos::remove(name);
        refresh();
        return;
    }

    // OK was pressed
    QString newName = dialog.getValue(nameField.key);
    SettingsMazeAlgos::update(
        name,
        newName,
        dialog.getValue(dirPathField.key),
        dialog.getValue(buildCommandField.key),
        dialog.getValue(runCommandField.key)
    );

    // Update the maze algos
    refresh(newName);
}

void MazeAlgosTab::build() {
    const QString& name = m_comboBox->currentText();
    ProcessUtilities::build(
        name,
        SettingsMazeAlgos::names(),
        SettingsMazeAlgos::getBuildCommand(name),
        SettingsMazeAlgos::getDirPath(name),
        m_buildDisplay,
        m_buildButton,
        this
    );
}

void MazeAlgosTab::run() {

	// TODO: upforgrabs
	// Deduplicate this logic with build()

	// Clear the run output
	if (m_runDisplay->autoClearCheckBox->isChecked()) {
		m_runDisplay->textEdit->clear();
	}

	// Perform some validation
    const QString& name = m_comboBox->currentText();
    if (!SettingsMazeAlgos::names().contains(name)) {
		m_runDisplay->textEdit->appendPlainText("[CORRUPT ALGORITHM CONFIG]\n");
        return;
    }
	QString runCommand = SettingsMazeAlgos::getRunCommand(name);
	if (runCommand.isEmpty()) {
		m_runDisplay->textEdit->appendPlainText("[EMPTY RUN COMMAND]\n");
        return;
	}
	QString dirPath = SettingsMazeAlgos::getDirPath(name);
	if (dirPath.isEmpty()) {
		m_runDisplay->textEdit->appendPlainText("[EMPTY DIRECTORY]\n");
        return;
	}

    // Instantiate a new process
    QProcess* process = new QProcess(this);

    // Display run output
    connect(process, &QProcess::readyReadStandardOutput, this, [=](){
        QString output = process->readAllStandardOutput();
        m_runDisplay->textEdit->appendPlainText(output);
    });

    // Re-enable run button when run finishes, clean up the process
    connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        [=](int exitCode, QProcess::ExitStatus exitStatus){
            m_runButton->setEnabled(true);
			if (exitStatus == QProcess::NormalExit && exitCode == 0) {
				m_runDisplay->textEdit->appendPlainText("[RUN COMPLETE]\n");
				QString output = process->readAllStandardError();
				emit mazeGenerated(output.toUtf8());
			}
			else {
				m_runDisplay->textEdit->appendPlainText("[RUN FAILED]\n");
			}
            delete process;
        }
    );
            
	// Update the random seed
    int seed = m_nextSeedBox->value();
    m_prevSeedBox->setValue(seed);
	m_nextSeedBox->setValue(getNextSeed());

    // Add the height, width, and seed to the command
    runCommand += " " + m_widthBox->cleanText();
    runCommand += " " + m_heightBox->cleanText();
    runCommand += " " + QString::number(seed);

    // GUI upkeep before run starts
    m_runButton->setEnabled(false);
    m_runDisplay->textEdit->appendPlainText(
		QString("[RUN INITIATED] - ") +
		QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss AP") +
		QString("\n")
	);

    // Start the run process
    bool success = ProcessUtilities::start(runCommand, dirPath, process);
    if (!success) {
        m_runButton->setEnabled(true);
        m_runDisplay->textEdit->appendPlainText(
            QString("[RUN FAILED TO START] - ") +
            process->errorString() +
            QString("\n")
        );
        delete process;
    }
}

void MazeAlgosTab::refresh(const QString& name) {
    m_comboBox->clear();
    for (const QString& algoName : SettingsMazeAlgos::names()) {
        m_comboBox->addItem(algoName);
    }
    int index = m_comboBox->findText(name);
    if (index != -1) {
        m_comboBox->setCurrentIndex(index);
    }
    bool isEmpty = (m_comboBox->count() == 0);
    m_comboBox->setEnabled(!isEmpty);
    m_editButton->setEnabled(!isEmpty);
    m_buildButton->setEnabled(!isEmpty);
    m_runButton->setEnabled(!isEmpty);
}

QVector<ConfigDialogField> MazeAlgosTab::getFields() {

    ConfigDialogField nameField;
    nameField.key = "NAME";
    nameField.label = "Name";

    ConfigDialogField dirPathField;
    dirPathField.key = "DIR_PATH";
    dirPathField.label = "Directory";
    dirPathField.fileBrowser = true;
    dirPathField.onlyDirectories = true;

    ConfigDialogField buildCommandField;
    buildCommandField.key = "BUILD_COMMAND";
    buildCommandField.label = "Build Command";

    ConfigDialogField runCommandField;
    runCommandField.key = "RUN_COMMAND";
    runCommandField.label = "Run Command";

    return {
        nameField,
        dirPathField,
        buildCommandField,
        runCommandField,
    };
}

int MazeAlgosTab::getNextSeed() {
	return SimUtilities::randomNonNegativeInt(MAX_SEED_VALUE + 1);
}

} // namespace mms
