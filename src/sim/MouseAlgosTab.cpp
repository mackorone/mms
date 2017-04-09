#include "MouseAlgosTab.h"

#include <limits>

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QTabWidget>
#include <QVBoxLayout>

#include "ConfigDialog.h"
#include "Param.h"
#include "ProcessUtilities.h"
#include "SettingsMouseAlgos.h"
#include "SimUtilities.h"

// TODO: MACK
#include "State.h"

namespace mms {

MouseAlgosTab::MouseAlgosTab() :
        m_comboBox(new QComboBox()),
        m_editButton(new QPushButton("Edit")),
        m_seedBox(new QSpinBox()),
        m_seedAutoUpdate(new QCheckBox("Auto-update")),
        m_buildButton(new QPushButton("Build")),
        m_buildDisplay(new TextDisplayWidget()),
        m_runButton(new QPushButton("Run")),
        m_runDisplay(new TextDisplayWidget()),
        m_stopButton(new QPushButton("Stop")),
        m_pauseButton(new QPushButton("Pause")) {

    // Set up the layout
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    QHBoxLayout* topLayout = new QHBoxLayout();
    layout->addLayout(topLayout);

    // Add the import button
    QPushButton* importButton = new QPushButton("Import Mouse Algorithm");
    connect(importButton, &QPushButton::clicked, this, &MouseAlgosTab::import);
    topLayout->addWidget(importButton);

    // Add the combobox
    topLayout->addWidget(m_comboBox);

    // Create the algo buttons
    connect(m_editButton, &QPushButton::clicked, this, &MouseAlgosTab::edit);
    connect(m_buildButton, &QPushButton::clicked, this, &MouseAlgosTab::build);
    connect(m_runButton, &QPushButton::clicked, this, &MouseAlgosTab::run);

    // Add the algo buttons to the tab
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_runButton);
    buttonsLayout->addWidget(m_buildButton);
    buttonsLayout->addWidget(m_editButton);
    layout->addLayout(buttonsLayout);

    // Add the "speed" buttons
    QHBoxLayout* speedsLayout = new QHBoxLayout();
    m_stopButton->setEnabled(false);
    m_pauseButton->setEnabled(false);
    speedsLayout->addWidget(m_stopButton);
    speedsLayout->addWidget(m_pauseButton);
    connect(m_stopButton, &QPushButton::clicked, this, [=](){
        emit stopRequested();
    });
    connect(m_pauseButton, &QPushButton::clicked, this, [=](){
        bool pause = m_pauseButton->text() == "Pause";
        m_pauseButton->setText(pause ? "Resume": "Pause");
        emit pauseButtonPressed(pause);
    });
    layout->addLayout(speedsLayout);

    // Add the "speed" slider
    double maxSpeed = P()->maxSimSpeed();
    QSlider* speedSlider = new QSlider(Qt::Horizontal);
    QDoubleSpinBox* speedBox = new QDoubleSpinBox();
    speedBox->setRange(maxSpeed / 100.0, maxSpeed);
    speedsLayout->addWidget(speedSlider);
    speedsLayout->addWidget(speedBox);
    connect(
        speedSlider, &QSlider::valueChanged,
        this, [=](int value){
            double fraction = (value + 1.0) / 100.0;
            speedBox->setValue(fraction * maxSpeed);
            emit simSpeedChanged(fraction * maxSpeed);
        }
    );
    connect(
        speedBox,
        static_cast<void(QDoubleSpinBox::*)()>(&QDoubleSpinBox::editingFinished),
        this, [=](){
            double percent = speedBox->value() / speedBox->maximum() * 100.0;
            speedSlider->setValue(static_cast<int>(percent - 1.0));
        }
    );
    speedSlider->setValue(100.0 / speedBox->maximum() - 1.0);

    // Add the mouse algo config box
    QGroupBox* optionsBox = new QGroupBox("Mouse Options");
    layout->addWidget(optionsBox);
    QGridLayout* optionsLayout = new QGridLayout();
    optionsBox->setLayout(optionsLayout);

    // Add the random seed config field
    QLabel* seedLabel = new QLabel("Previous Seed");
    seedLabel->setAlignment(Qt::AlignCenter);
    m_seedBox->setRange(0, std::numeric_limits<int>::max());
	m_seedBox->setValue(SimUtilities::randomNonNegativeInt());
	m_seedAutoUpdate->setCheckState(Qt::Checked);
    optionsLayout->addWidget(seedLabel, 0, 0);
    optionsLayout->addWidget(m_seedBox, 1, 0);
    optionsLayout->addWidget(m_seedAutoUpdate, 1, 1);
    connect(m_seedAutoUpdate, &QCheckBox::stateChanged, this, [=](int state){
        QString text = (state == Qt::Checked ? "Previous Seed" : "Seed");
        seedLabel->setText(text);
        m_seedBox->setReadOnly(state == Qt::Checked);
    });

    // Add the build output and run output
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

    // Add the mouse algos
    refresh();
}

void MouseAlgosTab::mouseAlgoStopped() {
    m_stopButton->setEnabled(false);
    m_pauseButton->setEnabled(false);
}

void MouseAlgosTab::import() {

    QVector<ConfigDialogField> fields = getFields();
    ConfigDialogField nameField = fields.at(0);
    ConfigDialogField dirPathField = fields.at(1);
    ConfigDialogField buildCommandField = fields.at(2);
    ConfigDialogField runCommandField = fields.at(3);
    ConfigDialogField mouseFilePathField = fields.at(4);

    ConfigDialog dialog(
        "Import",
        "Mouse Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField,
            mouseFilePathField,
        },
        false // No "Remove" button
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Ok was pressed
    QString name = dialog.getValue(nameField.key);
    SettingsMouseAlgos::add(
        name,
        dialog.getValue(dirPathField.key),
        dialog.getValue(buildCommandField.key),
        dialog.getValue(runCommandField.key),
        dialog.getValue(mouseFilePathField.key)
    );

    // Update the mouse algos
    refresh(name);
}

void MouseAlgosTab::edit() {

    QString name = m_comboBox->currentText();

    QVector<ConfigDialogField> fields = getFields();
    ConfigDialogField nameField = fields.at(0);
    ConfigDialogField dirPathField = fields.at(1);
    ConfigDialogField buildCommandField = fields.at(2);
    ConfigDialogField runCommandField = fields.at(3);
    ConfigDialogField mouseFilePathField = fields.at(4);

    nameField.initialValue = name;
    dirPathField.initialValue = SettingsMouseAlgos::getDirPath(name);
    buildCommandField.initialValue = SettingsMouseAlgos::getBuildCommand(name);
    runCommandField.initialValue = SettingsMouseAlgos::getRunCommand(name);
    mouseFilePathField.initialValue =
        SettingsMouseAlgos::getMouseFilePath(name);

    ConfigDialog dialog(
        "Edit",
        "Mouse Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField,
            mouseFilePathField,
        },
        true // Include a "Remove" button
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Remove was pressed
    if (dialog.removeButtonPressed()) {
        SettingsMouseAlgos::remove(name);
        refresh();
        return;
    }

    // OK was pressed
    QString newName = dialog.getValue(nameField.key);
    SettingsMouseAlgos::update(
        name,
        newName,
        dialog.getValue(dirPathField.key),
        dialog.getValue(buildCommandField.key),
        dialog.getValue(runCommandField.key),
        dialog.getValue(mouseFilePathField.key)
    );

    // Update the mouse algos
    refresh(newName);
}

void MouseAlgosTab::build() {
    const QString& name = m_comboBox->currentText();
	ProcessUtilities::build(
		name,
    	SettingsMouseAlgos::names(),
		SettingsMouseAlgos::getBuildCommand(name),
		SettingsMouseAlgos::getDirPath(name),
		m_buildDisplay,
		m_buildButton,
		this
	);
}

void MouseAlgosTab::run() {

    // TODO: MACK - this is a hack
    /*
    S()->setPaused(false);
    m_pauseButton->setText("Pause");
    */

	if (m_runDisplay->autoClearCheckBox->isChecked()) {
		m_runDisplay->textEdit->clear();
	}

	// Perform some validation
    const QString& name = m_comboBox->currentText();
    if (!SettingsMouseAlgos::names().contains(name)) {
		m_runDisplay->textEdit->appendPlainText("[CORRUPT ALGORITHM CONFIG]\n");
        return;
    }
	QString runCommand = SettingsMouseAlgos::getRunCommand(name);
	if (runCommand.isEmpty()) {
		m_runDisplay->textEdit->appendPlainText("[EMPTY RUN COMMAND]\n");
        return;
	}
	QString dirPath = SettingsMouseAlgos::getDirPath(name);
	if (dirPath.isEmpty()) {
		m_runDisplay->textEdit->appendPlainText("[EMPTY DIRECTORY]\n");
        return;
	}
	QString mouseFilePath = SettingsMouseAlgos::getMouseFilePath(name);
	if (mouseFilePath.isEmpty()) {
		m_runDisplay->textEdit->appendPlainText("[EMPTY MOUSE FILE]\n");
        return;
	}

	// Update the random seed
	if (m_seedAutoUpdate->isChecked()) {
		m_seedBox->setValue(SimUtilities::randomNonNegativeInt());
	}

    // Update some other UI components
    // TODO: MACK - this is a hack, doesn't work if maze file changed
    m_stopButton->setEnabled(true);
    m_pauseButton->setEnabled(true);

    // Once we're sure the necessary items exist, emit the signal
    emit mouseAlgoSelected(
        name,
        runCommand,
        dirPath,
        mouseFilePath,
        m_seedBox->value(),
        m_runDisplay
    );
}

void MouseAlgosTab::refresh(const QString& name) {
    m_comboBox->clear();
    for (const QString& algoName : SettingsMouseAlgos::names()) {
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

QVector<ConfigDialogField> MouseAlgosTab::getFields() {

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

    ConfigDialogField mouseFilePathField;
    mouseFilePathField.key = "MOUSE_FILE_PATH";
    mouseFilePathField.label = "Mouse File";
    mouseFilePathField.fileBrowser = true;

    return {
        nameField,
        dirPathField,
        buildCommandField,
        runCommandField,
        mouseFilePathField,
    };
}

} // namespace mms
