#include "Window.h"

#include <QAction>
#include <QFrame>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>

#include "MazeAlgosTab.h"
#include "MazeFilesTab.h"
#include "Model.h"
#include "MouseAlgosTab.h"
#include "Param.h"
#include "TextDisplay.h"

namespace mms {

Window::Window(QWidget *parent) :
        QMainWindow(parent),
        m_truthButton(new QRadioButton("Truth")),
        m_viewButton(new QRadioButton("Mouse")),
        m_wallTruthCheckbox(new QCheckBox("Walls")),
        m_colorCheckbox(new QCheckBox("Color")),
        m_fogCheckbox(new QCheckBox("Fog")),
        m_textCheckbox(new QCheckBox("Text")),
        m_zoomCheckbox(new QCheckBox("Zoom")),
        m_maze(nullptr),
        m_truth(nullptr),
        m_mouse(nullptr),
        m_mouseGraphic(nullptr),
        m_view(nullptr),
        m_controller(nullptr),
        m_mouseAlgoThread(nullptr) {

    // First, start the physics loop
    QObject::connect(
        &m_modelThread, &QThread::started,
        &m_model, &Model::simulate);
    m_model.moveToThread(&m_modelThread);
    m_modelThread.start();

    // Add the splitter to the window
    QSplitter* splitter = new QSplitter();
    splitter->setHandleWidth(6);
    setCentralWidget(splitter);

	// Add some generic menu items
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* saveMazeAction = new QAction(tr("&Save Maze As ..."), this);
    connect(saveMazeAction, &QAction::triggered, this, [=](){
        // TODO: MACK
	});
	fileMenu->addAction(saveMazeAction);
    QAction* quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, this, [=](){
		close();
	});
	fileMenu->addAction(quitAction);

	// Add some model-related menu items
    QMenu* simulationMenu = menuBar()->addMenu(tr("&Simulation"));
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

    // Add a container for the map and options
    QWidget* mapHolder = new QWidget();
    QVBoxLayout* mapHolderLayout = new QVBoxLayout();
    mapHolder->setLayout(mapHolderLayout);
    splitter->addWidget(mapHolder);

    // Add the map
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
    mapOptionsLayout->addWidget(m_wallTruthCheckbox);
    mapOptionsLayout->addWidget(m_colorCheckbox);
    mapOptionsLayout->addWidget(m_fogCheckbox);
    mapOptionsLayout->addWidget(m_textCheckbox);
    mapOptionsLayout->addWidget(m_zoomCheckbox);

    // Add functionality to those map buttons
    connect(m_viewButton, &QRadioButton::toggled, this, [=](bool checked){
		m_map.setView(checked ? m_view : m_truth);
        m_wallTruthCheckbox->setEnabled(checked);
        m_colorCheckbox->setEnabled(checked);
        m_fogCheckbox->setEnabled(checked);
        m_textCheckbox->setEnabled(checked);
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
    connect(m_zoomCheckbox, &QCheckBox::stateChanged, this, [=](int state){
        if (m_view != nullptr) {
            m_map.setLayoutType(
                state == Qt::Checked
                ? LayoutType::ZOOMED
                : LayoutType::FULL);
        }
    });

    // Set the default values for the map options
    m_truthButton->setChecked(true);
    m_viewButton->setEnabled(false);
    m_wallTruthCheckbox->setChecked(false);
    m_wallTruthCheckbox->setEnabled(false);
    m_colorCheckbox->setChecked(true);
    m_colorCheckbox->setEnabled(false);
    m_fogCheckbox->setChecked(true);
    m_fogCheckbox->setEnabled(false);
    m_textCheckbox->setChecked(true);
    m_textCheckbox->setEnabled(false);
    m_zoomCheckbox->setChecked(false);
    m_zoomCheckbox->setEnabled(false);
    
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
    MazeAlgosTab* mazeAlgosTab = new MazeAlgosTab();
    connect(
        mazeAlgosTab, &MazeAlgosTab::mazeGenerated,
        this, [=](const QByteArray& bytes){
            Maze* maze = Maze::fromAlgo(bytes);
            if (maze != nullptr) {
                setMaze(maze);
            }
        }
    );
    tabWidget->addTab(mazeAlgosTab, "Maze Algorithms");

    // Create the mouse algos tab
    MouseAlgosTab* mouseAlgosTab = new MouseAlgosTab();
    connect(
        mouseAlgosTab, &MouseAlgosTab::mouseAlgoSelected,
        this, &Window::runMouseAlgo
    );
    connect(
        mouseAlgosTab, &MouseAlgosTab::mouseAlgoSelected,
        this, [=](){
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

    // Resize some things
    resize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    m_map.resize(m_map.height(), m_map.height());
}

void Window::closeEvent(QCloseEvent *event) {
	// Graceful shutdown
	stopMouseAlgo();
	m_modelThread.quit();
	m_model.shutdown();
	m_modelThread.wait();
	m_map.shutdown();
    QMainWindow::closeEvent(event);
}

void Window::setMaze(Maze* maze) {

    // First, stop the mouse algo
    stopMouseAlgo();

    // Next, update the maze and truth
    Maze* oldMaze = m_maze;
    MazeView* oldTruth = m_truth;
    m_maze = maze;
    m_truth = new MazeView(
		m_maze,
		true, // wallTruthVisible
		false, // tileColorsVisible
		false, // tileFogVisible
		true, // tileTextVisible
		true // autopopulateTextWithDistance
	);

    // Update pointers held by other objects
    m_model.setMaze(m_maze);
    m_map.setMaze(m_maze);
    m_map.setView(m_truth);

    // Delete the old objects
    delete oldMaze;
    delete oldTruth;
}

void Window::runMouseAlgo(
        const QString& name,
        const QString& runCommand,
        const QString& dirPath,
        const QString& mouseFilePath,
        int seed,
        TextDisplay* display) {

    // If maze is empty, raise an error
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
            "The mouse file could not be loaded."
        );
        delete newMouse;
        return;
    }

    // Kill the current mouse algorithm
    stopMouseAlgo();

    // Update some random UI components
    m_viewButton->setEnabled(true);
    m_viewButton->setChecked(true);
    m_zoomCheckbox->setEnabled(true);

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
    Controller* newController = new Controller(m_maze, newMouse, newView);

    // Listen for mouse algo stdout
    connect(
        newController, &Controller::algoStdout,
        display, &QPlainTextEdit::appendPlainText
    );

    // The thread on which the controller will execute
    QThread* newMouseAlgoThread = new QThread();

    // We need to actually spawn the algorithm's process (via start()) in the
    // separate thread, hence why this is async. Note that we need the separate
    // thread because, while it's performing an algorithm-requested action, the
    // Controller could block the GUI loop from executing.
    connect(newMouseAlgoThread, &QThread::started, newController, [=](){
        // We need to add the mouse to the world *after* the the controller is
        // initialized (thus ensuring that tile fog is cleared automatically),
        // but *before* we actually start the algorithm (lest the mouse
        // position/orientation not be updated properly during the beginning of
        // the mouse algo's execution)
        newController->init(&m_model);
        m_model.addMouse("", newMouse);
        newController->start(name);
    });

    // When the thread finishes, clean everything up
    connect(newMouseAlgoThread, &QThread::finished, this, [=](){
        delete newController;
        delete newMouseAlgoThread;
        delete newMouseGraphic;
        delete newView;
        delete newMouse;
    });

    // Update the member variables
    m_mouse = newMouse;
    m_view = newView;
    m_mouseGraphic = newMouseGraphic;
    m_controller = newController;
    m_mouseAlgoThread = newMouseAlgoThread;

    // Update the map to use the algorithm's view
    m_map.setView(m_view);
    m_map.setMouseGraphic(m_mouseGraphic);

    // Start the controller thread
    m_controller->moveToThread(m_mouseAlgoThread);
	m_mouseAlgoThread->start();
}

void Window::stopMouseAlgo() {
    // If there is no controller, there is no algo
    if (m_controller == nullptr) {
        return;
    }
    // Request the event loop to stop
    m_mouseAlgoThread->quit();
    // Quickly return control to the event loop
    m_controller->requestStop();
    // Wait for the event loop to actually stop
    m_mouseAlgoThread->wait();
    // At this point, no more mouse functions will execute
    m_controller = nullptr;
    m_map.setMouseGraphic(nullptr);
    m_model.removeMouse("");
    // Update some random UI components
    m_truthButton->setChecked(true);
    m_viewButton->setEnabled(false);
    m_zoomCheckbox->setEnabled(false);
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
    if (m_controller != nullptr) {
        if (m_controller->getInterfaceType(false) == InterfaceType::DISCRETE) {
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
        {"Random Seed", P()->randomSeed()},
        // {"Mouse Algo", P()->mouseAlgorithm()}, // TODO: MACK
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
        // TODO: MACK - get this from the controller
        // {"Mouse Algo", P()->mouseAlgorithm()},
        /*
        {"Mouse File", (
            m_controller == nullptr
            ? "NONE"
            : m_controller->getStaticOptions().mouseFile
        }),
        */
        // TODO: MACK - interface type not finalized
        {"Interface Type",
            m_controller == nullptr
            ? "NONE"
            : INTERFACE_TYPE_TO_STRING.value(m_controller->getInterfaceType(false))
        },
        /*
        QString("Initial Direction:           ") + (m_controller == nullptr ? "NONE" :
            m_controller->getStaticOptions().initialDirection),
        QString("Tile Text Num Rows:          ") + (m_controller == nullptr ? "NONE" :
            QString::number(m_controller->getStaticOptions().tileTextNumberOfRows)),
        QString("Tile Text Num Cols:          ") + (m_controller == nullptr ? "NONE" :
            QString::number(m_controller->getStaticOptions().tileTextNumberOfCols)),
        */
        {"Allow Omniscience",
            m_controller == nullptr
            ? "NONE"
            : m_controller->getDynamicOptions().allowOmniscience ? "TRUE" : "FALSE"
        },
        {"Auto Clear Fog",
            m_controller == nullptr
            ? "NONE"
            : m_controller->getDynamicOptions().automaticallyClearFog ? "TRUE" : "FALSE"
        },
        {"Declare Both Wall Halves",
            m_controller == nullptr
            ? "NONE"
            : m_controller->getDynamicOptions().declareBothWallHalves ? "TRUE" : "FALSE"
        },
        {"Auto Set Tile Text",
            m_controller == nullptr
            ? "NONE"
            : m_controller->getDynamicOptions().setTileTextWhenDistanceDeclared ? "TRUE" : "FALSE"
        },
        {"Auto Set Tile Base Color",
            m_controller == nullptr
            ? "NONE"
            : m_controller->getDynamicOptions().setTileBaseColorWhenDistanceDeclaredCorrectly ? "TRUE" : "FALSE"
        }
        // TODO: MACK
        /*
        QString("Wheel Speed Fraction:        ") +
            (m_controller == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_controller->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            QString::number(m_controller->getStaticOptions().wheelSpeedFraction))),
        QString("Declare Wall On Read:        ") +
            (m_controller == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_controller->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_controller->getDynamicOptions().declareWallOnRead ? "TRUE" : "FALSE"))),
        QString("Use Tile Edge Movements:     ") +
            (m_controller == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_controller->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_controller->getDynamicOptions().useTileEdgeMovements ? "TRUE" : "FALSE"))),
        */
    };
}

QVector<QPair<QString, QVariant>> Window::getMazeInfo() const {
    return {
        // TODO: MACK
        /*
        {
            (P()->useMazeFile() ? "Maze File" : "Maze Algo"),
            (P()->useMazeFile() ? P()->mazeFile() : P()->mazeAlgorithm()),
        },
        */
        {"Maze Width", m_maze->getWidth()},
        {"Maze Height", m_maze->getHeight()},
        {"Maze Is Official", m_maze->isOfficialMaze() ? "TRUE" : "FALSE"},
    };
}

QVector<QPair<QString, QVariant>> Window::getOptions() const {
    return {
        // Sim state
        {"Layout Type (l)", LAYOUT_TYPE_TO_STRING.value(S()->layoutType())},
        {"Rotate Zoomed Map (r)", (S()->rotateZoomedMap() ? "TRUE" : "FALSE")},
        {"Zoomed Map Scale (i, o)", QString::number(S()->zoomedMapScale())},
        {"Wall Truth Visible (t)", (S()->wallTruthVisible() ? "TRUE" : "FALSE")},
        {"Tile Colors Visible (c)", (S()->tileColorsVisible() ? "TRUE" : "FALSE")},
        {"Tile Fog Visible (g)", (S()->tileFogVisible() ? "TRUE" : "FALSE")},
        {"Tile Text Visible (x)", (S()->tileTextVisible() ? "TRUE" : "FALSE")},
        {"Tile Distance Visible (d)", (S()->tileDistanceVisible() ? "TRUE" : "FALSE")},
        {"Paused (p)", (S()->paused() ? "TRUE" : "FALSE")},
        {"Sim Speed (f, s)", QString::number(S()->simSpeed())},
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

} // namespace mms
