#include "Window.h"

#include <QAction>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>

#include "ConfigDialog.h"
#include "MazeFilesTab.h"
#include "Model.h"
#include "Param.h"
#include "ProcessUtilities.h"
#include "SettingsMazeAlgos.h"
#include "SettingsMouseAlgos.h"
#include "SimUtilities.h"

namespace mms {

Window::Window(QWidget *parent) :
        QMainWindow(parent),
        m_mazeWidthLabel(new QLabel()),
        m_mazeHeightLabel(new QLabel()),
        m_maxDistanceLabel(new QLabel()),
        m_mazeDirLabel(new QLabel()),
        m_isValidLabel(new QLabel()),
        m_isOfficialLabel(new QLabel()),
        m_truthButton(new QRadioButton("Truth")),
        m_viewButton(new QRadioButton("Mouse")),
        m_distancesCheckbox(new QCheckBox("Distance")),
        m_wallTruthCheckbox(new QCheckBox("Walls")),
        m_colorCheckbox(new QCheckBox("Color")),
        m_fogCheckbox(new QCheckBox("Fog")),
        m_textCheckbox(new QCheckBox("Text")),
        m_followCheckbox(new QCheckBox("Follow")),
        m_maze(nullptr),
        m_truth(nullptr),
        m_mouse(nullptr),
        m_mouseGraphic(nullptr),
        m_view(nullptr),
        m_mouseInterface(nullptr),
        m_mouseAlgoThread(nullptr),

        // TODO: MACK - MazeAlgosTab
        m_mazeAlgoWidget(new QWidget()),
        m_mazeAlgoComboBox(new QComboBox()),
        m_mazeAlgoEditButton(new QPushButton("Edit")),
        m_mazeAlgoImportButton(new QPushButton("Import")),
        m_mazeAlgoBuildProcess(nullptr),
        m_mazeAlgoBuildButton(new QPushButton("Build")),
        m_mazeAlgoBuildStatus(new QLabel()),
        m_mazeAlgoBuildOutput(new QPlainTextEdit()),
        m_mazeAlgoRunProcess(nullptr),
        m_mazeAlgoRunButton(new QPushButton("Run")),
        m_mazeAlgoRunStatus(new QLabel()),
        m_mazeAlgoRunOutput(new QPlainTextEdit()),
        m_mazeAlgoWidthBox(new QSpinBox()),
        m_mazeAlgoHeightBox(new QSpinBox()),
        m_mazeAlgoSeedWidget(new RandomSeedWidget()),

        // TODO: MACK - MouseAlgosTab
        m_mouseAlgoWidget(new QWidget()),
        m_mouseAlgoComboBox(new QComboBox()),
        m_mouseAlgoEditButton(new QPushButton("Edit")),
        m_mouseAlgoImportButton(new QPushButton("Import")),
        m_mouseAlgoBuildProcess(nullptr),
        m_mouseAlgoBuildButton(new QPushButton("Build")),
        m_mouseAlgoBuildStatus(new QLabel()),
        m_mouseAlgoBuildOutput(new QPlainTextEdit()),
        // TODO: MACK - are all of these necessary?
        m_mouseAlgoRunProcess(nullptr),
        m_mouseAlgoRunButton(new QPushButton("Run")),
        m_mouseAlgoRunStatus(new QLabel()),
        m_mouseAlgoRunOutput(new QPlainTextEdit()),
        m_mouseAlgoSeedWidget(new RandomSeedWidget()) {

    // First, start the physics loop
    connect(
        &m_modelThread, &QThread::started,
        &m_model, &Model::simulate);
    m_model.moveToThread(&m_modelThread);
    m_modelThread.start();

    // Add the splitter to the window
    QSplitter* splitter = new QSplitter();
    splitter->setHandleWidth(6);
    setCentralWidget(splitter);

    // Add a container for the maze stats, map and options
    QWidget* mapHolder = new QWidget();
    QVBoxLayout* mapHolderLayout = new QVBoxLayout();
    mapHolder->setLayout(mapHolderLayout);
    splitter->addWidget(mapHolder);

    // Add the maze stats
	QWidget* mazeStatsBox = new QWidget();
    QHBoxLayout* mazeStatsLayout = new QHBoxLayout();
    mazeStatsBox->setLayout(mazeStatsLayout);
    mapHolderLayout->addWidget(mazeStatsBox);
    for (QPair<QString, QLabel*> pair : QVector<QPair<QString, QLabel*>> {
        {"Width", m_mazeWidthLabel},
        {"Height", m_mazeHeightLabel},
        {"Max", m_maxDistanceLabel},
        {"Start", m_mazeDirLabel},
        {"Valid", m_isValidLabel},
        {"Official", m_isOfficialLabel},
    }) {
        pair.second->setAlignment(Qt::AlignCenter);   
        pair.second->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        QLabel* label = new QLabel(pair.first);
        mazeStatsLayout->addWidget(label);
        mazeStatsLayout->addWidget(pair.second);
    }

    // Add the map (and set some layout props)
    mapHolderLayout->addWidget(&m_map);
    mapHolderLayout->setContentsMargins(0, 0, 0, 0);
    mapHolderLayout->setSpacing(0);
    m_map.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Add the map options
	QWidget* mapOptionsBox = new QWidget();
    QHBoxLayout* mapOptionsLayout = new QHBoxLayout();
    mapOptionsBox->setLayout(mapOptionsLayout);
    mapHolderLayout->addWidget(mapOptionsBox);
    mapOptionsLayout->addWidget(m_truthButton);
    mapOptionsLayout->addWidget(m_viewButton);
    mapOptionsLayout->addWidget(m_distancesCheckbox);
    mapOptionsLayout->addWidget(m_wallTruthCheckbox);
    mapOptionsLayout->addWidget(m_colorCheckbox);
    mapOptionsLayout->addWidget(m_fogCheckbox);
    mapOptionsLayout->addWidget(m_textCheckbox);
    mapOptionsLayout->addWidget(m_followCheckbox);

    // Add functionality to those map buttons
    connect(m_viewButton, &QRadioButton::toggled, this, [=](bool checked){
		m_map.setView(checked ? m_view : m_truth);
        m_distancesCheckbox->setEnabled(!checked);
        m_wallTruthCheckbox->setEnabled(checked);
        m_colorCheckbox->setEnabled(checked);
        m_fogCheckbox->setEnabled(checked);
        m_textCheckbox->setEnabled(checked);
    });
    connect(m_distancesCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_truth != nullptr) {
            m_truth->getMazeGraphic()->setTileTextVisible(state == Qt::Checked);
        }
    });
    connect(m_wallTruthCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_view != nullptr) {
            m_view->getMazeGraphic()->setWallTruthVisible(state == Qt::Checked);
        }
    });
    connect(m_colorCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_view != nullptr) {
            m_view->getMazeGraphic()->setTileColorsVisible(state == Qt::Checked);
        }
    });
    connect(m_fogCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_view != nullptr) {
            m_view->getMazeGraphic()->setTileFogVisible(state == Qt::Checked);
        }
    });
    connect(m_textCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_view != nullptr) {
            m_view->getMazeGraphic()->setTileTextVisible(state == Qt::Checked);
        }
    });
    connect(m_followCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_view != nullptr) {
            m_map.setLayoutType(
                state == Qt::Checked
                ? LayoutType::ZOOMED
                : LayoutType::FULL);
        }
    });

    // Set the default values for the map options
    m_truthButton->setChecked(true);
    m_distancesCheckbox->setChecked(true);
    m_distancesCheckbox->setEnabled(true);
    m_viewButton->setEnabled(false);
    m_wallTruthCheckbox->setChecked(false);
    m_wallTruthCheckbox->setEnabled(false);
    m_colorCheckbox->setChecked(true);
    m_colorCheckbox->setEnabled(false);
    m_fogCheckbox->setChecked(true);
    m_fogCheckbox->setEnabled(false);
    m_textCheckbox->setChecked(true);
    m_textCheckbox->setEnabled(false);
    m_followCheckbox->setChecked(false);
    m_followCheckbox->setEnabled(false);
    
    // Add the tabs to the splitter
    QTabWidget* tabWidget = new QTabWidget();
    splitter->addWidget(tabWidget);

    // Create the maze files tab
    MazeFilesTab* mazeFilesTab = new MazeFilesTab(); 
    connect(
        mazeFilesTab, &MazeFilesTab::mazeFileChanged,
        this, [=](const QString& path){
            Maze* maze = Maze::fromFile(path);
            if (maze != nullptr) {
                setMaze(maze);
            }
        }
    );
    tabWidget->addTab(mazeFilesTab, "Maze Files");

    // Create the maze algos tab
    mazeAlgoTabInit();
    tabWidget->addTab(m_mazeAlgoWidget, "Maze Algorithms");

    // Create the mouse algos tab
    mouseAlgoTabInit();
    tabWidget->addTab(m_mouseAlgoWidget, "Mouse Algorithms");

	// Add some generic menu items
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    /*
    QAction* saveMazeAction = new QAction(tr("&Save Maze As ..."), this);
    connect(saveMazeAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	fileMenu->addAction(saveMazeAction);
    */
    QAction* quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, this, [=](){
		close();
	});
	fileMenu->addAction(quitAction);

	// Add maze algorithm menu items
    /*
    QMenu* mazeAlgoMenu = menuBar()->addMenu(tr("&Maze Algorithm"));
    QAction* importAction = new QAction(tr("&Import"), this);
    // TODO: MACK - just to maze algo tab here?
    connect(
        importAction, &QAction::triggered,
        mazeAlgosTab, &MazeAlgosTab::import
    );
	mazeAlgoMenu->addAction(importAction);
    QAction* editAction = new QAction(tr("&Edit"), this);
    connect(editAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	mazeAlgoMenu->addAction(editAction);
    QAction* buildAction = new QAction(tr("&Build"), this);
    connect(buildAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	mazeAlgoMenu->addAction(buildAction);
    QAction* runAction = new QAction(tr("&Run"), this);
    connect(runAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	mazeAlgoMenu->addAction(runAction);

	// Add some model-related menu items
    QMenu* simulationMenu = menuBar()->addMenu(tr("&Mouse"));
    QAction* pauseAction = new QAction(tr("&Pause"), this);
    connect(pauseAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	simulationMenu->addAction(pauseAction);
    QAction* stopAction = new QAction(tr("&Stop"), this);
    connect(stopAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	simulationMenu->addAction(stopAction);
    */

    // Resize the window
    resize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
}

void Window::closeEvent(QCloseEvent *event) {
	// Graceful shutdown
    // TODO: MACK - stop all other currently executing processes, too
	mouseAlgoRunStop();
	m_modelThread.quit();
	m_model.shutdown();
	m_modelThread.wait();
	m_map.shutdown();
    QMainWindow::closeEvent(event);
}

void Window::setMaze(Maze* maze) {

	// TODO: MACK - stop all processes, like maze building and what not

    // First, stop the mouse algo
    mouseAlgoRunStop();

    // Next, update the maze and truth
    Maze* oldMaze = m_maze;
    MazeView* oldTruth = m_truth;
    m_maze = maze;
    m_truth = new MazeView(
		m_maze,
		true, // wallTruthVisible
		false, // tileColorsVisible
		false, // tileFogVisible
		m_distancesCheckbox->isChecked(), // tileTextVisible
		true // autopopulateTextWithDistance
	);

    // Update pointers held by other objects
    m_model.setMaze(m_maze);
    m_map.setMaze(m_maze);
    m_map.setView(m_truth);

    // Update maze stats UI widgets
    m_mazeWidthLabel->setText(QString::number(m_maze->getWidth()));
    m_mazeHeightLabel->setText(QString::number(m_maze->getHeight()));
    m_maxDistanceLabel->setText(
        QString::number(m_maze->getMaximumDistance())
    );
    m_mazeDirLabel->setText(
        DIRECTION_TO_STRING.value(m_maze->getOptimalStartingDirection())
    );
    m_isValidLabel->setText(m_maze->isValidMaze() ? "TRUE" : "FALSE");
    m_isOfficialLabel->setText(m_maze->isOfficialMaze() ? "TRUE" : "FALSE");

    // Delete the old objects
    delete oldMaze;
    delete oldTruth;
}

void Window::algoActionStart(
	QProcess** actionProcessVariable,
	QPushButton* actionButton,
	QLabel* actionStatus,
	QPlainTextEdit* actionOutput,
    QTabWidget* outputTabWidget,
	const QString& algoName,
	const QString& actionName,
	const QString& actionString,
	QString (*getCommand)(const QString&),
	QString (*getDirPath)(const QString&),
	QVector<QString> (Window::*getExtraArgs)(void),
	void (Window::*actionStart)(void),
	void (Window::*actionStop)(void),
	void (Window::*stderrMidAction)(void),
	void (Window::*stderrPostAction)(void)
) {
    // The action should not be running
    ASSERT_FA(actionProcessVariable == nullptr);
    ASSERT_TR(*actionProcessVariable == nullptr);

	// Exactly one function should be supplied for handling stderr
	ASSERT_NE(
		(stderrMidAction == nullptr),
		(stderrPostAction == nullptr)
	);

    // Get the command and directory for the action
    QString command = getCommand(algoName);
    QString dirPath = getDirPath(algoName);

	// Perform some validation
	if (command.isEmpty()) {
        QMessageBox::warning(
            this,
            QString("Empty %1 Command").arg(actionName),
            QString("%1 command for algorithm \"%2\" is empty.").arg(
				actionName,
				algoName
			)
        );
        return;
	}
	if (dirPath.isEmpty()) {
        QMessageBox::warning(
            this,
            QString("Empty Directory"),
            QString("Directory for algorithm \"%1\" is empty.").arg(
				algoName
			)
        );
        return;
	}

	// Clear the action ouput
	actionOutput->clear();

    // Instantiate a new process
    QProcess* process = new QProcess(this);

    // Display action output
    connect(process, &QProcess::readyReadStandardOutput, this, [=](){
        QString output = process->readAllStandardOutput();
        if (output.endsWith("\n")) {
            output.truncate(output.size() - 1);
        }
        actionOutput->appendPlainText(output);
    });

	// If configured, handle stderr during the action
	if (stderrMidAction != nullptr) {
    	connect(
			process, &QProcess::readyReadStandardError,
			this, stderrMidAction
		);
	}

    // Re-enable build button when build finishes, clean up the process
    connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        [=](int exitCode, QProcess::ExitStatus exitStatus){

			// Set the button to "Action"
            disconnect(actionButton, &QPushButton::clicked, this, actionStop);
            connect(actionButton, &QPushButton::clicked, this, actionStart);
            actionButton->setText(actionName);

			// Update the status label, call stderrPostAction
            if (exitStatus == QProcess::NormalExit && exitCode == 0) {
				if (stderrPostAction != nullptr) {
					(this->*stderrPostAction)();
				}
                actionStatus->setText("COMPLETE");
                actionStatus->setStyleSheet(
                    "QLabel { background: rgb(150, 255, 100); }"
                );
            }
            else {
                actionStatus->setText("FAILED");
                actionStatus->setStyleSheet(
                    "QLabel { background: rgb(255, 150, 150); }"
                );
            }

            // Clean up the process
            delete *actionProcessVariable;
            *actionProcessVariable = nullptr;
        }
    );

	// Set the button to "Cancel"
    disconnect(actionButton, &QPushButton::clicked, this, actionStart);
    connect(actionButton, &QPushButton::clicked, this, actionStop);
    actionButton->setText("Cancel");

	// Update the status label
    actionStatus->setText(actionString);
    actionStatus->setStyleSheet(
        "QLabel { background: rgb(255, 255, 100); }"
    );

	// Get extra arguments for the command
	QVector<QString> args;
	if (getExtraArgs != nullptr) {
		for (const QString& arg : (this->*getExtraArgs)()) {
			command += QString(" %1").arg(arg);
		}
	}

    // Switch to the correct output tab
    outputTabWidget->setCurrentWidget(actionOutput);

    // Start the build process
    bool started = ProcessUtilities::start(command, dirPath, process);
    if (started) {
        *actionProcessVariable = process;
    }
    else {
	    // Set the button to "Action"
        disconnect(actionButton, &QPushButton::clicked, this, actionStop);
        connect(actionButton, &QPushButton::clicked, this, actionStart);
        actionButton->setText(actionName);

	    // Update the status label
        actionStatus->setText("ERROR");
        actionStatus->setStyleSheet(
            "QLabel { background: rgb(255, 150, 150); }"
        );
        actionOutput->appendPlainText(process->errorString());

        // Delete the process
        delete process;
    }
}

void Window::algoActionStop(
	QProcess* actionProcess,
	QLabel* actionStatus
) {
    actionProcess->terminate();
    actionProcess->waitForFinished();
    actionStatus->setText("CANCELED");
}

#if(0)
void Window::keyPress(int key) {
    // NOTE: If you're adding or removing anything from this function, make
    // sure to update wiki/Keys.md
    if (
        key == Qt::Key_0 || key == Qt::Key_1 ||
        key == Qt::Key_2 || key == Qt::Key_3 ||
        key == Qt::Key_4 || key == Qt::Key_5 ||
        key == Qt::Key_6 || key == Qt::Key_7 ||
        key == Qt::Key_8 || key == Qt::Key_9
    ) {
        // Press an input button
        int inputButton = key - Qt::Key_0;
        if (!S()->inputButtonWasPressed(inputButton)) {
            S()->setInputButtonWasPressed(inputButton, true);
            qInfo().noquote().nospace()
                << "Input button " << inputButton << " was pressed.";
        }
        else {
            qWarning().noquote().nospace()
                << "Input button " << inputButton << " has not yet been"
                << " acknowledged as pressed; pressing it has no effect.";
        }
    }
}

void Window::togglePause() {
    if (m_mouseInterface != nullptr) {
        if (m_mouseInterface->getInterfaceType(false) == InterfaceType::DISCRETE) {
            if (S()->paused()) {
                S()->setPaused(false);
                ui.pauseButton->setText("Pause");
            }
            else {
                S()->setPaused(true);
                ui.pauseButton->setText("Resume");
            }
        }
        else {
            qWarning().noquote().nospace()
                << "Pausing the simulator is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
    }
}

QVector<QPair<QString, QVariant>> Window::getRunStats() const {

    MouseStats stats;
    if (m_model.containsMouse("")) {
        stats = m_model.getMouseStats("");
    }

    return {
        {"Tiles Traversed",
            QString::number(stats.traversedTileLocations.size()) + " / " +
            QString::number(m_maze->getWidth() * m_maze->getHeight())
        },
        {"Closest Distance to Center", stats.closestDistanceToCenter},
        {"Current X (m)", m_mouse->getCurrentTranslation().getX().getMeters()},
        {"Current Y (m)", m_mouse->getCurrentTranslation().getY().getMeters()},
        {"Current Rotation (deg)", m_mouse->getCurrentRotation().getDegreesZeroTo360()},
        {"Current X tile", m_mouse->getCurrentDiscretizedTranslation().first},
        {"Current Y tile", m_mouse->getCurrentDiscretizedTranslation().second},
        {"Current Direction",
            DIRECTION_TO_STRING.value(m_mouse->getCurrentDiscretizedRotation())
        },
        {"Elapsed Real Time", SimUtilities::formatDuration(SimTime::get()->elapsedRealTime())},
        {"Elapsed Sim Time", SimUtilities::formatDuration(SimTime::get()->elapsedSimTime())},
        {"Time Since Origin Departure",
            stats.timeOfOriginDeparture.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(
                SimTime::get()->elapsedSimTime() - stats.timeOfOriginDeparture)
        },
        {"Best Time to Center",
            stats.bestTimeToCenter.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(stats.bestTimeToCenter)
        },
        {"Crashed", (S()->crashed() ? "TRUE" : "FALSE")},
    };
}

QVector<QPair<QString, QVariant>> Window::getAlgoOptions() const {
    return {
        // Mouse Info
        // TODO: MACK - get this from the mouseInterface
        // {"Mouse Algo", P()->mouseAlgorithm()},
        /*
        {"Mouse File", (
            m_mouseInterface == nullptr
            ? "NONE"
            : m_mouseInterface->getStaticOptions().mouseFile
        }),
        */
        // TODO: MACK - interface type not finalized
        {"Interface Type",
            m_mouseInterface == nullptr
            ? "NONE"
            : INTERFACE_TYPE_TO_STRING.value(m_mouseInterface->getInterfaceType(false))
        },
        /*
        QString("Initial Direction:           ") + (m_mouseInterface == nullptr ? "NONE" :
            m_mouseInterface->getStaticOptions().initialDirection),
        QString("Tile Text Num Rows:          ") + (m_mouseInterface == nullptr ? "NONE" :
            QString::number(m_mouseInterface->getStaticOptions().tileTextNumberOfRows)),
        QString("Tile Text Num Cols:          ") + (m_mouseInterface == nullptr ? "NONE" :
            QString::number(m_mouseInterface->getStaticOptions().tileTextNumberOfCols)),
        */
        {"Allow Omniscience",
            m_mouseInterface == nullptr
            ? "NONE"
            : m_mouseInterface->getDynamicOptions().allowOmniscience ? "TRUE" : "FALSE"
        },
        {"Auto Clear Fog",
            m_mouseInterface == nullptr
            ? "NONE"
            : m_mouseInterface->getDynamicOptions().automaticallyClearFog ? "TRUE" : "FALSE"
        },
        {"Declare Both Wall Halves",
            m_mouseInterface == nullptr
            ? "NONE"
            : m_mouseInterface->getDynamicOptions().declareBothWallHalves ? "TRUE" : "FALSE"
        },
        {"Auto Set Tile Text",
            m_mouseInterface == nullptr
            ? "NONE"
            : m_mouseInterface->getDynamicOptions().setTileTextWhenDistanceDeclared ? "TRUE" : "FALSE"
        },
        {"Auto Set Tile Base Color",
            m_mouseInterface == nullptr
            ? "NONE"
            : m_mouseInterface->getDynamicOptions().setTileBaseColorWhenDistanceDeclaredCorrectly ? "TRUE" : "FALSE"
        }
        // TODO: MACK
        /*
        QString("Wheel Speed Fraction:        ") +
            (m_mouseInterface == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_mouseInterface->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            QString::number(m_mouseInterface->getStaticOptions().wheelSpeedFraction))),
        QString("Declare Wall On Read:        ") +
            (m_mouseInterface == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_mouseInterface->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_mouseInterface->getDynamicOptions().declareWallOnRead ? "TRUE" : "FALSE"))),
        QString("Use Tile Edge Movements:     ") +
            (m_mouseInterface == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_mouseInterface->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_mouseInterface->getDynamicOptions().useTileEdgeMovements ? "TRUE" : "FALSE"))),
        */
    };
}


    // TODO: MACK - group the position stuff together
    /*
    // Add run stats info to the UI
    QVector<QPair<QString, QVariant>> runStats = getRunStats();
    for (int i = 0; i < runStats.size(); i += 1) {
        QString label = runStats.at(i).first;
        QLabel* labelHolder = new QLabel(label + ":");
        QLabel* valueHolder = new QLabel();
        ui->runStatsLayout->addWidget(labelHolder, i, 0);
        ui->runStatsLayout->addWidget(valueHolder, i, 1);
        m_runStats.insert(label, valueHolder);
    }

    // Add run stats info to the UI
    QVector<QPair<QString, QVariant>> mazeInfo = getMazeInfo();
    for (int i = 0; i < mazeInfo.size(); i += 1) {
        QString label = mazeInfo.at(i).first;
        QLabel* labelHolder = new QLabel(label + ":");
        QLabel* valueHolder = new QLabel();
        ui->mazeInfoLayout->addWidget(labelHolder, i, 0);
        ui->mazeInfoLayout->addWidget(valueHolder, i, 1);
        m_mazeInfo.insert(label, valueHolder);
    }

    // Periodically update the header
    connect(
        &m_headerRefreshTimer,
        &QTimer::timeout,
        this,
        [=](){
            // TODO: MACK - only update if tab is visible
            QVector<QPair<QString, QVariant>> runStats = getRunStats();
            for (const auto& pair : runStats) {
                QString text = pair.second.toString();
                if (pair.second.type() == QVariant::Double) {
                    text = QString::number(pair.second.toDouble(), 'f', 3);
                }
                m_runStats.value(pair.first)->setText(text);
            }
            QVector<QPair<QString, QVariant>> mazeInfo = getMazeInfo();
            for (const auto& pair : mazeInfo) {
                QString text = pair.second.toString();
                if (pair.second.type() == QVariant::Double) {
                    text = QString::number(pair.second.toDouble(), 'f', 3);
                }
                m_mazeInfo.value(pair.first)->setText(text);
            }
        }
    );
    m_headerRefreshTimer.start(50);
    */
#endif

void Window::mazeAlgoTabInit() {

    // First, set up all of the button connections
    connect(
        m_mazeAlgoEditButton, &QPushButton::clicked,
        this, &Window::mazeAlgoEdit
    );
    connect(
        m_mazeAlgoImportButton, &QPushButton::clicked,
        this, &Window::mazeAlgoImport
    );
    connect(
        m_mazeAlgoBuildButton, &QPushButton::clicked,
        this, &Window::mazeAlgoBuildStart
    );
    connect(
        m_mazeAlgoRunButton, &QPushButton::clicked,
        this, &Window::mazeAlgoRunStart
    );

    // Next, set up the layout
    QVBoxLayout* layout = new QVBoxLayout();
    m_mazeAlgoWidget->setLayout(layout);
    QGridLayout* topLayout = new QGridLayout();
    layout->addLayout(topLayout);

    // Create a combobox for the algorithm options
    QGroupBox* algorithmGroupBox = new QGroupBox("Algorithm");
    topLayout->addWidget(algorithmGroupBox, 0, 0);
    QGridLayout* algorithmLayout = new QGridLayout();
    algorithmGroupBox->setLayout(algorithmLayout);
    algorithmLayout->addWidget(m_mazeAlgoComboBox, 0, 0, 1, 2);
    algorithmLayout->addWidget(m_mazeAlgoImportButton, 1, 0);
    algorithmLayout->addWidget(m_mazeAlgoEditButton, 1, 1);

    // Actions groupbox
    QGroupBox* actionsGroupBox = new QGroupBox("Actions");
    topLayout->addWidget(actionsGroupBox, 1, 0);
    QGridLayout* actionsLayout = new QGridLayout();
    actionsGroupBox->setLayout(actionsLayout);
    actionsLayout->addWidget(m_mazeAlgoBuildButton, 0, 0);
    actionsLayout->addWidget(m_mazeAlgoBuildStatus, 0, 1);
    actionsLayout->addWidget(m_mazeAlgoRunButton, 1, 0);
    actionsLayout->addWidget(m_mazeAlgoRunStatus, 1, 1);
    for (QLabel* label : {m_mazeAlgoBuildStatus, m_mazeAlgoRunStatus}) {
        label->setAlignment(Qt::AlignCenter);   
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        label->setMinimumWidth(90);
    }

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
    m_mazeAlgoWidthBox->setValue(16);
    m_mazeAlgoHeightBox->setValue(16);
    mazeSizeLayout->addWidget(new QLabel("Width"));
    mazeSizeLayout->addWidget(m_mazeAlgoWidthBox);
    mazeSizeLayout->addWidget(new QLabel("Height"));
    mazeSizeLayout->addWidget(m_mazeAlgoHeightBox);

    // Add the random seed box
    optionsLayout->addWidget(m_mazeAlgoSeedWidget);

    // Add the build and run output
    m_mazeAlgoOutputTabWidget = new QTabWidget();
    layout->addWidget(m_mazeAlgoOutputTabWidget);
	m_mazeAlgoOutputTabWidget->addTab(m_mazeAlgoBuildOutput, "Build Output");
	m_mazeAlgoOutputTabWidget->addTab(m_mazeAlgoRunOutput, "Run Output");

    // Set the default values for some widgets
    for (QPlainTextEdit* output : {
        m_mazeAlgoBuildOutput,
        m_mazeAlgoRunOutput,
    }) {
        output->setReadOnly(true);
        output->setLineWrapMode(QPlainTextEdit::NoWrap);
        // TODO: upforgrabs
        // The line wrap mode should be configurable
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        // TODO: upforgrabs
        // This font size should be configurable
        font.setPointSize(10);
        output->document()->setDefaultFont(font);
    }

    // Add the maze algos
    mazeAlgoRefresh();
}

void Window::mazeAlgoImport() {

    QVector<ConfigDialogField> fields = mazeAlgoGetFields();
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
    mazeAlgoRefresh(name);
}

void Window::mazeAlgoEdit() {

    QString name = m_mazeAlgoComboBox->currentText();

    QVector<ConfigDialogField> fields = mazeAlgoGetFields();
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
        mazeAlgoRefresh();
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
    mazeAlgoRefresh(newName);
}

void Window::mazeAlgoBuildStart() {
	algoActionStart(
		&m_mazeAlgoBuildProcess,
		m_mazeAlgoBuildButton,
		m_mazeAlgoBuildStatus,
		m_mazeAlgoBuildOutput,
        m_mazeAlgoOutputTabWidget,
    	m_mazeAlgoComboBox->currentText(),
		"Build",
		"BUILDING",
		SettingsMazeAlgos::getBuildCommand,
		SettingsMazeAlgos::getDirPath,
		nullptr,
		&Window::mazeAlgoBuildStart,
		&Window::mazeAlgoBuildStop,
		&Window::mazeAlgoBuildStderr,
		nullptr
	);
}

void Window::mazeAlgoBuildStop() {
	algoActionStop(
    	m_mazeAlgoBuildProcess,
		m_mazeAlgoBuildStatus
	);
}

void Window::mazeAlgoBuildStderr() {
	ASSERT_FA(m_mazeAlgoBuildProcess == nullptr);
	QString error = m_mazeAlgoBuildProcess->readAllStandardError();
	if (error.endsWith("\n")) {
		error.truncate(error.size() - 1);
	}
	m_mazeAlgoBuildOutput->appendPlainText(error);
}

QVector<QString> Window::mazeAlgoRunExtraArgs() {
	return {
		m_mazeAlgoWidthBox->cleanText(),
		m_mazeAlgoHeightBox->cleanText(),
		QString::number(m_mazeAlgoSeedWidget->next()),
	};
}

void Window::mazeAlgoRunStart() {
	algoActionStart(
		&m_mazeAlgoRunProcess,
		m_mazeAlgoRunButton,
		m_mazeAlgoRunStatus,
		m_mazeAlgoRunOutput,
        m_mazeAlgoOutputTabWidget,
    	m_mazeAlgoComboBox->currentText(),
		"Run",
		"RUNNING",
		SettingsMazeAlgos::getRunCommand,
		SettingsMazeAlgos::getDirPath,
		&Window::mazeAlgoRunExtraArgs,
		&Window::mazeAlgoRunStart,
		&Window::mazeAlgoRunStop,
		nullptr,
		&Window::mazeAlgoRunStderr
	);
}

void Window::mazeAlgoRunStop() {
	algoActionStop(
    	m_mazeAlgoRunProcess,
		m_mazeAlgoRunStatus
	);
}

void Window::mazeAlgoRunStderr() {
	ASSERT_FA(m_mazeAlgoRunProcess == nullptr);
	QString output = m_mazeAlgoRunProcess->readAllStandardError();
	Maze* maze = Maze::fromAlgo(output.toUtf8());
	if (maze != nullptr) {
		setMaze(maze);
	}
	else {
        QMessageBox::warning(
            this,
            "Invalid Maze",
            "Could not load generated maze"
        );
	}
}

void Window::mazeAlgoRefresh(const QString& name) {
    m_mazeAlgoComboBox->clear();
    for (const QString& algoName : SettingsMazeAlgos::names()) {
        m_mazeAlgoComboBox->addItem(algoName);
    }
    int index = m_mazeAlgoComboBox->findText(name);
    if (index != -1) {
        m_mazeAlgoComboBox->setCurrentIndex(index);
    }
    bool isEmpty = (m_mazeAlgoComboBox->count() == 0);
    m_mazeAlgoComboBox->setEnabled(!isEmpty);
    m_mazeAlgoEditButton->setEnabled(!isEmpty);
    m_mazeAlgoBuildButton->setEnabled(!isEmpty);
    m_mazeAlgoRunButton->setEnabled(!isEmpty);
}

QVector<ConfigDialogField> Window::mazeAlgoGetFields() {

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

void Window::mouseAlgoTabInit() {

    // First, set up all of the button connections
    connect(
        m_mouseAlgoEditButton, &QPushButton::clicked,
        this, &Window::mouseAlgoEdit
    );
    connect(
        m_mouseAlgoImportButton, &QPushButton::clicked,
        this, &Window::mouseAlgoImport
    );
    connect(
        m_mouseAlgoBuildButton, &QPushButton::clicked,
        this, &Window::mouseAlgoBuildStart
    );
    connect(
        m_mouseAlgoRunButton, &QPushButton::clicked,
        this, &Window::mouseAlgoRunStart
    );

    // TODO: MACK - dedup this with maze algo

    // Set up the layout
    QVBoxLayout* layout = new QVBoxLayout();
    m_mouseAlgoWidget->setLayout(layout);
    QGridLayout* topLayout = new QGridLayout();
    layout->addLayout(topLayout);

    // Create a combobox for the algorithm options
    QGroupBox* algorithmGroupBox = new QGroupBox("Algorithm");
    topLayout->addWidget(algorithmGroupBox, 0, 0);
    QGridLayout* algorithmLayout = new QGridLayout();
    algorithmGroupBox->setLayout(algorithmLayout);
    algorithmLayout->addWidget(m_mouseAlgoComboBox, 0, 0);
    algorithmLayout->addWidget(m_mouseAlgoImportButton, 1, 0);
    algorithmLayout->addWidget(m_mouseAlgoEditButton, 1, 1);

    // Actions groupbox
    QGroupBox* actionsGroupBox = new QGroupBox("Actions");
    topLayout->addWidget(actionsGroupBox, 1, 0);
    QGridLayout* actionsLayout = new QGridLayout();
    actionsGroupBox->setLayout(actionsLayout);
    actionsLayout->addWidget(m_mouseAlgoBuildButton, 0, 0);
    actionsLayout->addWidget(m_mouseAlgoBuildStatus, 0, 1);
    actionsLayout->addWidget(m_mouseAlgoRunButton, 1, 0);
    actionsLayout->addWidget(m_mouseAlgoRunStatus, 1, 1);
    for (QLabel* label : {m_mouseAlgoBuildStatus, m_mouseAlgoRunStatus}) {
        label->setAlignment(Qt::AlignCenter);   
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        label->setMinimumWidth(90);
    }

    // TODO: MACK
    /*
    connect(
        mouseAlgosTab, &MouseAlgosTab::pauseButtonPressed,
        this, [=](bool pause){
            m_model.setPaused(pause);
        }
    );
	connect(
		mouseAlgosTab, &MouseAlgosTab::simSpeedChanged,
		this, [=](double factor){
            // We have to call the function on this UI thread (as opposed to
            // hooking up the signal directly to the slot) because the Model
            // thread is blocked on simulate(), and thus never processes the
            // signal
			m_model.setSimSpeed(factor);
		}
	);
    tabWidget->addTab(mouseAlgosTab, "Mouse Algorithms");
    */
    /*
    // Add the "speed" buttons
    QHBoxLayout* speedsLayout = new QHBoxLayout();
    m_pauseButton->setEnabled(false);
    //speedsLayout->addWidget(m_stopButton);
    speedsLayout->addWidget(m_pauseButton);
    //connect(m_stopButton, &QPushButton::clicked, this, [=](){
    //    emit stopRequested();
    //});
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

    // Add the input buttons
    QHBoxLayout* inputButtonsLayout = new QHBoxLayout();
    inputButtonsLayout->addWidget(new QLabel("Input Buttons"));
    for (int i = 0; i < 10; i += 1) {
        QPushButton* button = new QPushButton(QString::number(i));
        button->setMinimumSize(3, 0);
        button->setCheckable(true);
        inputButtonsLayout->addWidget(button);
    }
    layout->addLayout(inputButtonsLayout);

    // Add the random seed config field
    layout->addWidget(m_seedWidget);
    */

    // Add the build and run output
    m_mouseAlgoOutputTabWidget = new QTabWidget();
    layout->addWidget(m_mouseAlgoOutputTabWidget);
	m_mouseAlgoOutputTabWidget->addTab(m_mouseAlgoBuildOutput, "Build Output");
	m_mouseAlgoOutputTabWidget->addTab(m_mouseAlgoRunOutput, "Run Output");

    // Set the default values for some widgets
    for (QPlainTextEdit* output : {
        m_mouseAlgoBuildOutput,
        m_mouseAlgoRunOutput,
    }) {
        output->setReadOnly(true);
        output->setLineWrapMode(QPlainTextEdit::NoWrap);
        // TODO: upforgrabs
        // The line wrap mode should be configurable
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        // TODO: upforgrabs
        // This font size should be configurable
        font.setPointSize(10);
        output->document()->setDefaultFont(font);
    }

    // Add the mouse algos
    mouseAlgoRefresh();
}

void Window::mouseAlgoEdit() {

    // TODO: MACK - dedup edit and import between maze and mouse

    QString name = m_mouseAlgoComboBox->currentText();

    QVector<ConfigDialogField> fields = mouseAlgoGetFields();
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
        mouseAlgoRefresh();
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
    mouseAlgoRefresh(newName);
}

void Window::mouseAlgoImport() {

    QVector<ConfigDialogField> fields = mouseAlgoGetFields();
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
    mouseAlgoRefresh(name);
}

void Window::mouseAlgoBuildStart() {
	algoActionStart(
		&m_mouseAlgoBuildProcess,
		m_mouseAlgoBuildButton,
		m_mouseAlgoBuildStatus,
		m_mouseAlgoBuildOutput,
        m_mouseAlgoOutputTabWidget,
    	m_mouseAlgoComboBox->currentText(),
		"Build",
		"BUILDING",
		SettingsMouseAlgos::getBuildCommand,
		SettingsMouseAlgos::getDirPath,
		nullptr,
		&Window::mouseAlgoBuildStart,
		&Window::mouseAlgoBuildStop,
		&Window::mouseAlgoBuildStderr,
		nullptr
	);
}

void Window::mouseAlgoBuildStop() {
	algoActionStop(
    	m_mouseAlgoBuildProcess,
		m_mouseAlgoBuildStatus
	);
}

void Window::mouseAlgoBuildStderr() {
	ASSERT_FA(m_mouseAlgoBuildProcess == nullptr);
	QString error = m_mouseAlgoBuildProcess->readAllStandardError();
	if (error.endsWith("\n")) {
		error.truncate(error.size() - 1);
	}
	m_mouseAlgoBuildOutput->appendPlainText(error);
}

QVector<QString> Window::mouseAlgoRunExtraArgs() {
    // TODO: MACK
}

void Window::mouseAlgoRunStart() {

    // TODO: Cancel/stop button not working just yet
    // TODO: MACK - dedup this validation with the other process functions
    
    QString algoName = m_mouseAlgoComboBox->currentText();
	QString dirPath = SettingsMouseAlgos::getDirPath(algoName);
	QString command = SettingsMouseAlgos::getRunCommand(algoName);
	QString mouseFilePath = SettingsMouseAlgos::getMouseFilePath(algoName);

    // TODO: MACK - add the random seed to command...

	// Perform config validation
	if (command.isEmpty()) {
        QMessageBox::warning(
            this,
            "Empty Run Command",
            QString("Run command for algorithm \"%1\" is empty.").arg(
				algoName
			)
        );
        return;
	}
	if (dirPath.isEmpty()) {
        QMessageBox::warning(
            this,
            "Empty Directory",
            QString("Directory for algorithm \"%1\" is empty.").arg(
				algoName
			)
        );
        return;
	}
	if (mouseFilePath.isEmpty()) {
        QMessageBox::warning(
            this,
            "Empty Mouse File",
            QString("Mouse file for algorithm \"%1\" is empty.").arg(
				algoName
			)
        );
        return;
	}

    // If maze is empty, abort
    if (m_maze == nullptr) {
        QMessageBox::warning(
            this,
            "No Maze",
            "You must load a maze before running a mouse algorithm."
        );
        return;
    }

    // Generate the mouse, check mouse file success
    Mouse* newMouse = new Mouse(m_maze);
    bool success = newMouse->reload(mouseFilePath);
    if (!success) {
        QMessageBox::warning(
            this,
            "Invalid Mouse File",
            QString("Mouse file \"%1\"could not be loaded.").arg(
                mouseFilePath
            )
        );
        delete newMouse;
        return;
    }

    // Kill the current mouse algorithm
    mouseAlgoRunStop();

    // Create some more objects
    MazeView* newView = new MazeView(
		m_maze,
		m_wallTruthCheckbox->isChecked(),
		m_colorCheckbox->isChecked(),
		m_fogCheckbox->isChecked(),
		m_textCheckbox->isChecked(),
		false // autopopulateTextWithDistance
	);
    MouseGraphic* newMouseGraphic = new MouseGraphic(newMouse);
    MouseInterface* newMouseInterface = new MouseInterface(
        m_maze,
        newMouse,
        newView
    );

    // Clear the output
    m_mouseAlgoRunOutput->clear();

    // The thread on which the mouse interface will execute
    QThread* newMouseAlgoThread = new QThread();

    // Instantiate the algorithm's QProcess object in a separate thread to
    // prevent the Controller from blocking the GUI loop while performing an
    // algorithm-requested action.
    connect(newMouseAlgoThread, &QThread::started, newMouseInterface, [=](){
        
        // Create the subprocess on which we'll execute the mouse algorithm
        QProcess* newProcess = new QProcess();

        // Listen for mouse algo stdout
        connect(newProcess, &QProcess::readyReadStandardOutput, this, [=](){
            QString output = newProcess->readAllStandardOutput();
            if (output.endsWith("\n")) {
                output.truncate(output.size() - 1);
            }
            m_mouseAlgoRunOutput->appendPlainText(output);
        });

        // Process all stderr commands as appropriate
        connect(
            newProcess,
            &QProcess::readyReadStandardError,
            // Handle the process's stderr on the mouse's event loop to
            // prevent the UI from freezing during a blocking mouse action
            newMouseInterface,
            [=](){
                QString text = newProcess->readAllStandardError();
                QStringList lines = getLines(text, &m_stderrBuffer);
                for (const QString& line : lines) {
                    QString response = newMouseInterface->dispatch(line);
                    if (!response.isEmpty()) {
                        newProcess->write((response + "\n").toStdString().c_str());
                    }
                }
            }
        );

        // First, connect the newTileLocationTraversed signal to a lambda that
        // clears tile fog *before* adding the mouse to the maze. This ensures
        // that the first tile's fog is always cleared (the initial value of
        // automaticallyClearFog is true). This means that, if an algorithm
        // doesn't want to automatically clear tile fog, it'll have to disable
        // tile fog and then mark the first tile as foggy.

        // TODO: MACK - if I use this, then it crashes
        // Note that this runs
        // on the UI thread so that clearing the fog isn't blocked on algorithm
        // actions (e.g., move forward, sleep, etc.).
        connect(
            &m_model,
            &Model::newTileLocationTraversed,
            newMouseInterface,
            [=](int x, int y){
                if (newMouseInterface->getDynamicOptions().automaticallyClearFog) {
                    newView->getMazeGraphic()->setTileFogginess(x, y, false);
                }
            }
        );

        // We need to add the mouse to the world *after* the making the
        // previous connection (thus ensuring that tile fog is cleared
        // automatically), but *before* we actually start the algorithm (lest
        // the mouse position/orientation not be updated properly during the
        // beginning of the mouse algo's execution)
        m_model.setMouse(newMouse);

        // ------------------------
        // TODO: MACK

            // Re-enable build button when build finishes, clean up the process
            connect(
                newProcess,
                static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
                    &QProcess::finished
                ),
                this,
                [=](int exitCode, QProcess::ExitStatus exitStatus){

                    // Set the button to "Action"
                    disconnect(
                        m_mouseAlgoRunButton, &QPushButton::clicked,
                        this, &Window::mouseAlgoRunStop
                    );
                    connect(
                        m_mouseAlgoRunButton, &QPushButton::clicked,
                        this, &Window::mouseAlgoRunStart
                    );
                    m_mouseAlgoRunButton->setText("Run");

                    // Update the status label, call stderrPostAction
                    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
                        m_mouseAlgoRunStatus->setText("COMPLETE");
                        m_mouseAlgoRunStatus->setStyleSheet(
                            "QLabel { background: rgb(150, 255, 100); }"
                        );
                    }
                    else {
                        m_mouseAlgoRunStatus->setText("FAILED");
                        m_mouseAlgoRunStatus->setStyleSheet(
                            "QLabel { background: rgb(255, 150, 150); }"
                        );
                    }
                }
            );

        // ------------------------


        // When the thread finishes, clean everything up
        connect(newMouseAlgoThread, &QThread::finished, this, [=](){
            newProcess->terminate();
            newProcess->waitForFinished();
            delete newProcess;
            delete newMouseAlgoThread;
            delete newMouseInterface;
            delete newMouseGraphic;
            delete newView;
            delete newMouse;
        });

        // ---------------------------
        // TODO: MACK - extract this into it's own function ...

            // Set the button to "Cancel"
            disconnect(
                m_mouseAlgoRunButton, &QPushButton::clicked,
                this, &Window::mouseAlgoRunStart
            );
            connect(
                m_mouseAlgoRunButton, &QPushButton::clicked,
                this, &Window::mouseAlgoRunStop
            );
            m_mouseAlgoRunButton->setText("Cancel");

            // Update the status label
            m_mouseAlgoRunStatus->setText("RUNNING");
            m_mouseAlgoRunStatus->setStyleSheet(
                "QLabel { background: rgb(255, 255, 100); }"
            );
        // ---------------------------

        // If the process fails to start, stop the thread and cleanup
        bool success = ProcessUtilities::start(command, dirPath, newProcess);
        if (!success) {

        // TODO: MACK - this doesn't work, button never gets undone

        // ---------------------------
        // TODO: MACK - extract this into it's own function ...
                // Update the status label
                m_mouseAlgoRunStatus->setText("ERROR");
                m_mouseAlgoRunStatus->setStyleSheet(
                    "QLabel { background: rgb(255, 150, 150); }"
                );
                m_mouseAlgoRunOutput->appendPlainText(newProcess->errorString());
        // ---------------------------

            newMouseAlgoThread->quit();
            m_model.removeMouse();
            return;
        }

        // Update the member variables because, at this
        // point, the algorithm started successfully
        m_mouse = newMouse;
        m_view = newView;
        m_mouseGraphic = newMouseGraphic;
        m_mouseInterface = newMouseInterface;
        m_mouseAlgoThread = newMouseAlgoThread;
		m_mouseAlgoRunProcess = newProcess;
        m_map.setView(newView);
        m_map.setMouseGraphic(newMouseGraphic);

        // TODO: MACK
        m_viewButton->setEnabled(true);
        m_viewButton->setChecked(true);
        m_followCheckbox->setEnabled(true);
    });

    // Start the mouse interface thread
    newMouseInterface->moveToThread(newMouseAlgoThread);
	newMouseAlgoThread->start();
}

void Window::mouseAlgoRunStop() {

    // Only stop the algo thread if an algo is running
    if (m_mouseInterface != nullptr) {
        // If the mouse interface exists, the thread exists
        ASSERT_FA(m_mouseAlgoThread == nullptr);
        // Request the event loop to stop
        m_mouseAlgoThread->quit();
        // Quickly return control to the event loop
        m_mouseInterface->requestStop();
        // Wait for the event loop to actually stop
        m_mouseAlgoThread->wait();
        // At this point, no more mouse functions will execute
    }

    // Regardless of whether or not an algo is running, put the Window in a
    // "mouseless" state (note that the objects themselves get deleted in a
    // separate callback). Note that we do this *after* stopping the algo
    // thread so that we can be sure no more stderr will be emitted.
    m_stderrBuffer.clear();
    m_map.setMouseGraphic(nullptr);
    m_map.setView(m_truth);
    m_model.removeMouse();
	m_mouseAlgoRunProcess = nullptr;
    m_mouseAlgoThread = nullptr;
    m_mouseInterface = nullptr;
    m_mouseGraphic = nullptr;
    m_view = nullptr;
    m_mouse = nullptr;

    // First, update the UI
    m_truthButton->setChecked(true);
    m_viewButton->setEnabled(false);
    m_followCheckbox->setEnabled(false);
    // TODO: MACK - fix the background color
    m_mouseAlgoRunStatus->setText("");
    m_mouseAlgoRunStatus->setStyleSheet(
        "QLabel { background: rgb(255, 255, 255); }"
    );
    // TODO: MACK - change to the current tab
    // TODO: Stop/pause buttons
}

void Window::mouseAlgoRefresh(const QString& name) {
    m_mouseAlgoComboBox->clear();
    for (const QString& algoName : SettingsMouseAlgos::names()) {
        m_mouseAlgoComboBox->addItem(algoName);
    }
    int index = m_mouseAlgoComboBox->findText(name);
    if (index != -1) {
        m_mouseAlgoComboBox->setCurrentIndex(index);
    }
    bool isEmpty = (m_mouseAlgoComboBox->count() == 0);
    m_mouseAlgoComboBox->setEnabled(!isEmpty);
    m_mouseAlgoEditButton->setEnabled(!isEmpty);
    m_mouseAlgoBuildButton->setEnabled(!isEmpty);
    m_mouseAlgoRunButton->setEnabled(!isEmpty);
}

QVector<ConfigDialogField> Window::mouseAlgoGetFields() {

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

QStringList Window::getLines(const QString& text, QStringList* buffer) {

    // TODO: upforgrabs
    // Determine whether or not this function is perf sensitive. If so,
    // refactor this so that we're not copying QStrings between lists.

    // Separate the text by line
    QStringList parts = SimUtilities::splitLines(text);

    // We'll return list of complete lines
    QStringList lines;

    // If the text has at least one newline character, we definitely have a
    // complete line; combine it with the contents of the buffer and append
    // it to the list of lines to be returned
    if (1 < parts.size()) {
        lines.append(buffer->join("") + parts.at(0));
        buffer->clear();
    }

    // All newline-separated parts in the text are lines
    for (int i = 1; i < parts.size() - 1; i += 1) {
        lines.append(parts.at(i));
    }

    // Store the last part of the text (empty string if the text ended
    // with newline) in the buffer, to be combined with future input
    buffer->append(parts.at(parts.size() - 1));

    return lines;
}

} // namespace mms
