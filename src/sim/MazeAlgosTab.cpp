#include "MazeAlgosTab.h"

#include <limits>

#include <QDateTime>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QSpacerItem>
#include <QSpinBox>
#include <QSplitter>
#include <QVBoxLayout>

#include "ConfigDialog.h"
#include "ProcessUtilities.h"
#include "SettingsMazeAlgos.h"

namespace mms {

MazeAlgosTab::MazeAlgosTab() :
        m_comboBox(new QComboBox()),
        m_buildButton(new QPushButton("Build")),
        m_buildOutput(new TextDisplay()),
        m_buildAutoClear(new QCheckBox("Auto-clear")),
        m_runButton(new QPushButton("Run")),
        m_runOutput(new TextDisplay()),
		m_runAutoClear(new QCheckBox("Auto-clear")) {

	// Initialize the checkstates
	m_buildAutoClear->setCheckState(Qt::Checked);
	m_runAutoClear->setCheckState(Qt::Checked);

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
    QPushButton* editButton = new QPushButton("Edit");
    connect(editButton, &QPushButton::clicked, this, &MazeAlgosTab::edit);
    connect(m_buildButton, &QPushButton::clicked, this, &MazeAlgosTab::build);
    connect(m_runButton, &QPushButton::clicked, this, &MazeAlgosTab::run);

    // Add the algo buttons to the tab
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_runButton);
    buttonsLayout->addWidget(m_buildButton);
    buttonsLayout->addWidget(editButton);
    layout->addLayout(buttonsLayout);

    // Add the maze algo config box
    QGroupBox* optionsBox = new QGroupBox("Maze Options");
    layout->addWidget(optionsBox);
    QGridLayout* optionsLayout = new QGridLayout();
    optionsBox->setLayout(optionsLayout);

    // Add the width and height config fields
    QSpinBox* widthBox = new QSpinBox();
    QSpinBox* heightBox = new QSpinBox();
    widthBox->setValue(16);
    heightBox->setValue(16);
    QLabel* widthLabel = new QLabel("Width");
    QLabel* heightLabel = new QLabel("Height");
    widthLabel->setAlignment(Qt::AlignCenter);
    heightLabel->setAlignment(Qt::AlignCenter);
    optionsLayout->addWidget(widthLabel, 0, 0);
    optionsLayout->addWidget(widthBox, 1, 0);
    optionsLayout->addWidget(heightLabel, 0, 1);
    optionsLayout->addWidget(heightBox, 1, 1);

    // Add the random seed config field
    QSpinBox* seedBox = new QSpinBox();
    int min = std::numeric_limits<int>::min();
    int max = std::numeric_limits<int>::max();
    seedBox->setRange(min, max);
    QLabel* seedLabel = new QLabel("Seed");
    seedLabel->setAlignment(Qt::AlignCenter);
    optionsLayout->addWidget(seedLabel, 0, 2);
    optionsLayout->addWidget(seedBox, 1, 2);
    QCheckBox* updateSeedCheckbox = new QCheckBox("Auto-update");
    optionsLayout->addWidget(updateSeedCheckbox, 1, 3);
    connect(updateSeedCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        QString text = (state == Qt::Checked ? "Previous Seed" : "Seed");
        seedLabel->setText(text);
        seedBox->setReadOnly(state == Qt::Checked);
    });

    // Add a spliter for the outputs
    QSplitter* splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->setHandleWidth(6);
    layout->addWidget(splitter);

    // Add the build output and run output
	QVector<QPair<QString, TextDisplay*>> displays;
	for (int i = 0; i < 2; i += 1) {
		QString label = (i == 0 ? "Build Output" : "Run Output");
		QCheckBox* autoClear = (i == 0 ? m_buildAutoClear : m_runAutoClear);
		TextDisplay* textDisplay = (i == 0 ? m_buildOutput : m_runOutput);
		QWidget* container = new QWidget();
		QVBoxLayout* layout = new QVBoxLayout(container);
		layout->setContentsMargins(0, 0, 0, 0);
		QHBoxLayout* headerLayout = new QHBoxLayout();
		headerLayout->addWidget(new QLabel(label));
		QPushButton* clearButton = new QPushButton("Clear");
		connect(clearButton, &QPushButton::clicked, this, [=](){
			textDisplay->clear();
		});
		headerLayout->addWidget(clearButton);
		headerLayout->addWidget(autoClear);
		headerLayout->addSpacerItem(
			new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed)
		);
		layout->addLayout(headerLayout);
		layout->addWidget(textDisplay);
		splitter->addWidget(container);
	}

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
        m_comboBox->removeItem(m_comboBox->findText(name));
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

    // Determine the build parameters
    const QString& name = m_comboBox->currentText();
    if (!SettingsMazeAlgos::names().contains(name)) {
        return;
    }

    // Instantiate a new process
    QProcess* process = new QProcess(this);

    // Display build errors
    connect(process, &QProcess::readyReadStandardError, this, [=](){
        QString errors = process->readAllStandardError();
        m_buildOutput->appendPlainText(errors);
    });

    // Re-enable build button when build finishes, clean up the process
    connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        [=](int exitCode, QProcess::ExitStatus exitStatus){
            m_buildButton->setEnabled(true);
    		m_buildOutput->appendPlainText(
				exitStatus == QProcess::NormalExit && exitCode == 0
				? "[BUILD COMPLETE]\n"
				: "[BUILD FAILED]\n"
			);
            process->deleteLater();
        }
    );

    // Disable the build button before build starts
    m_buildButton->setEnabled(false);
	if (m_buildAutoClear->isChecked()) {
		m_buildOutput->clear();
	}
    m_buildOutput->appendPlainText(
		QString("[BUILD STARTED] - ") +
		QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss AP") +
		QString("\n")
	);

    // Start the build process
    bool success = ProcessUtilities::start(
        SettingsMazeAlgos::getBuildCommand(name),
        SettingsMazeAlgos::getDirPath(name),
        process
    );
    if (!success) {
        m_buildButton->setEnabled(true);
    	m_buildOutput->appendPlainText("[BUILD FAILED]\n");
        process->deleteLater();
    }
}

void MazeAlgosTab::run() {
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
