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
        m_importButton(new QPushButton("Import")),
        m_buildProcess(nullptr),
        m_startBuildButton(new QPushButton("Start")),
        m_stopBuildButton(new QPushButton("Stop")),
        m_buildDisplay(new TextDisplayWidget()),
        m_runProcess(nullptr),
        m_startRunButton(new QPushButton("Start")),
        m_stopRunButton(new QPushButton("Stop")),
        m_runDisplay(new TextDisplayWidget()),
        m_widthBox(new QSpinBox()),
        m_heightBox(new QSpinBox()),
        m_seedWidget(new RandomSeedWidget()) {

    // First, set up all of the button connections
    connect(m_editButton, &QPushButton::clicked, this, &MazeAlgosTab::edit);
    connect(m_importButton, &QPushButton::clicked, this, &MazeAlgosTab::import);
    connect(m_startBuildButton, &QPushButton::clicked, this, &MazeAlgosTab::startBuild);
    connect(m_stopBuildButton, &QPushButton::clicked, this, &MazeAlgosTab::stopBuild);
    connect(m_startRunButton, &QPushButton::clicked, this, &MazeAlgosTab::startRun);
    connect(m_stopRunButton, &QPushButton::clicked, this, &MazeAlgosTab::stopRun);

    // Next, set up the layout
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    QGridLayout* topLayout = new QGridLayout();
    layout->addLayout(topLayout);

    // Create a combobox for the algorithm options
    QGroupBox* algorithmGroupBox = new QGroupBox("Algorithm");
    topLayout->addWidget(algorithmGroupBox, 0, 0, 1, 2);
    QGridLayout* algorithmLayout = new QGridLayout();
    algorithmGroupBox->setLayout(algorithmLayout);
    algorithmLayout->addWidget(m_comboBox, 0, 0, 1, 2);
    algorithmLayout->addWidget(m_editButton, 1, 0, 1, 1);
    algorithmLayout->addWidget(m_importButton, 1, 1, 1, 1);

    // Build groupbox
    QGroupBox* buildGroupBox = new QGroupBox("Build");
    topLayout->addWidget(buildGroupBox, 1, 0, 1, 1);
    QVBoxLayout* buildLayout = new QVBoxLayout();
    buildGroupBox->setLayout(buildLayout);
    buildLayout->addWidget(m_startBuildButton);
    buildLayout->addWidget(m_stopBuildButton);

    // Run groupbox
    QGroupBox* runGroupBox = new QGroupBox("Run");
    topLayout->addWidget(runGroupBox, 1, 1, 1, 1);
    QVBoxLayout* runLayout = new QVBoxLayout();
    runGroupBox->setLayout(runLayout);
    runLayout->addWidget(m_startRunButton);
    runLayout->addWidget(m_stopRunButton);

    // Options groupbox
    QGroupBox* optionsGroupBox = new QGroupBox("Options");
    topLayout->addWidget(optionsGroupBox, 0, 2, 2, 2);
    QVBoxLayout* optionsLayout = new QVBoxLayout();
    optionsGroupBox->setLayout(optionsLayout);

    // Add the maze size box
    QGroupBox* mazeSizeBox = new QGroupBox("Maze Size");
    optionsLayout->addWidget(mazeSizeBox);
    QHBoxLayout* mazeSizeLayout = new QHBoxLayout();
    mazeSizeBox->setLayout(mazeSizeLayout);

    // Add the maze size inputs
    m_widthBox->setValue(16);
    m_heightBox->setValue(16);
    mazeSizeLayout->addWidget(new QLabel("Width"));
    mazeSizeLayout->addWidget(m_widthBox);
    mazeSizeLayout->addWidget(new QLabel("Height"));
    mazeSizeLayout->addWidget(m_heightBox);

    // Add the random seed box
    optionsLayout->addWidget(m_seedWidget);

    // Add the build and run output
    QTabWidget* tabWidget = new QTabWidget();
    layout->addWidget(tabWidget);
	tabWidget->addTab(m_buildDisplay, "Build Output");
	tabWidget->addTab(m_runDisplay, "Run Output");
    connect(m_startBuildButton, &QPushButton::clicked, this, [=](){
        tabWidget->setCurrentWidget(m_buildDisplay);
    });
    connect(m_startRunButton, &QPushButton::clicked, this, [=](){
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

void MazeAlgosTab::startBuild() {
    // TODO: MACK - what happens if this finishes first?
    // TODO: MACK - process gets cleaned up ... whoops
    if (m_buildProcess != nullptr) {
        m_buildProcess->terminate();
        m_buildProcess->waitForFinished();
        //m_startBuildButton->setText("&Build");
        m_buildProcess = nullptr;
        return;
    }
    const QString& name = m_comboBox->currentText();
    m_buildProcess = ProcessUtilities::build(
        name,
        SettingsMazeAlgos::names(),
        SettingsMazeAlgos::getBuildCommand(name),
        SettingsMazeAlgos::getDirPath(name),
        m_buildDisplay,
        m_startBuildButton,
        this
    );
    //m_startBuildButton->setText("Cancel &Build");
}

void MazeAlgosTab::stopBuild() {
    // TODO: MACK
}

void MazeAlgosTab::startRun() {

	// TODO: upforgrabs
	// Deduplicate this logic with startBuild()

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

	// Clear the run output
	if (m_runDisplay->autoClearCheckBox->isChecked()) {
		m_runDisplay->textEdit->clear();
	}

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
            m_startRunButton->setEnabled(true);
			if (exitStatus == QProcess::NormalExit && exitCode == 0) {
				m_runDisplay->textEdit->appendPlainText("[RUN COMPLETE]\n");
				QString output = process->readAllStandardError();
				emit mazeGenerated(output.toUtf8());
			}
			else {
				m_runDisplay->textEdit->appendPlainText("[RUN FAILED]\n");
                // TODO: MACK
                m_runDisplay->textEdit->appendPlainText(process->errorString());
                m_runDisplay->textEdit->appendPlainText(QString::number(exitCode));

			}
            delete process;
        }
    );
            
    // Add the height, width, and seed to the command
    runCommand += " " + m_widthBox->cleanText();
    runCommand += " " + m_heightBox->cleanText();
    runCommand += " " + QString::number(m_seedWidget->next());

    // GUI upkeep before run starts
    m_startRunButton->setEnabled(false);
    m_runDisplay->textEdit->appendPlainText(
		QString("[RUN INITIATED] - ") +
		QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss AP") +
		QString("\n")
	);

    // Start the run process
    bool success = ProcessUtilities::start(runCommand, dirPath, process);
    if (!success) {
        m_startRunButton->setEnabled(true);
        m_runDisplay->textEdit->appendPlainText(
            QString("[RUN FAILED TO START] - ") +
            process->errorString() +
            QString("\n")
        );
        delete process;
    }

    // Update the member variable
    m_runProcess = process;
}

void MazeAlgosTab::stopRun() {
    // TODO: MACK
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
    m_startBuildButton->setEnabled(!isEmpty);
    m_startRunButton->setEnabled(!isEmpty);
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

} // namespace mms
