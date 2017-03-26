#include "Window.h"

#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>

#include "MazeAlgosTab.h"
#include "MazeFilesTab.h"
#include "Model.h"
#include "MouseAlgosTab.h"
#include "Param.h"
#include "TextDisplay.h"

namespace mms {

Window::Window(QWidget *parent) :
        QMainWindow(parent),
        m_maze(nullptr),
        m_truth(nullptr),
        m_mouse(nullptr),
        m_mouseGraphic(nullptr),
        m_view(nullptr),
        m_controller(nullptr),
        m_mouseAlgoThread(nullptr) {

    // Add the splitter to the window
    QSplitter* splitter = new QSplitter();
    splitter->setHandleWidth(6);
    setCentralWidget(splitter);

    // Add the map to the splitter
    splitter->addWidget(&m_map);
    
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
    tabWidget->addTab(mouseAlgosTab, "Mouse Algorithms");

    // Resize some things
    resize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    m_map.resize(m_map.height(), m_map.height());

    /*
    // TODO: MACK
    connect(ui.editParametersButton, &QPushButton::clicked, this, [](){
        ParamParser::execEditDialog();
    });

    // Connect pause button
    connect(ui.pauseButton, &QPushButton::clicked, this, &Window::togglePause);
    */
}

void Window::setMaze(Maze* maze) {

    // TODO: MACK - clean this up

    Maze* prev_maze = m_maze;
    MazeView* prev_truth = m_truth;

    m_maze = maze;
    m_truth = new MazeView(m_maze);
    m_map.setMaze(m_maze);
    m_map.setView(m_truth);

    stopMouseAlgo();

    Model::get()->setMaze(m_maze);
    delete prev_maze;
    delete prev_truth;
}

void Window::runMouseAlgo(
        const QString& name,
        const QString& runCommand,
        const QString& dirPath,
        const QString& mouseFilePath,
        int seed,
        TextDisplay* display) {

    // TODO: MACK - deal with empty maze

    // Generate the mouse, lens, and controller
    m_mouse = new Mouse(m_maze);
    bool success = m_mouse->reload(mouseFilePath);
    if (!success) {
        QMessageBox::warning(
            this,
            "Invalid Mouse File",
            // TODO: MACK - provide a reason here
            QString("The mouse file \n\n\"") + mouseFilePath + "\"\n\ncould not be loaded."
        );
        delete m_mouse;
        return;
    }

    // Kill the current mouse algorithm
    stopMouseAlgo();

    m_view = new MazeViewMutable(m_maze);
    m_mouseGraphic = new MouseGraphic(m_mouse);
    m_controller = new Controller(m_maze, m_mouse, m_view);

    // Configures the window to listen for build and run stdout,
    // as forwarded by the controller, and adds a map to the UI

    /////////////////////////////////

    // Add a map to the UI
    // TODO: MACK - minimum size, size policy
    m_map.setView(m_view);
    m_map.setMouseGraphic(m_mouseGraphic);

    // Listen for mouse algo stdout
    connect(
        m_controller,
        &Controller::algoStdout,
        display,
        &QPlainTextEdit::appendPlainText
    );

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
    /////////////////////////////////

    // TODO: MACK
    Mouse* mouse = m_mouse;
    MazeViewMutable* view = m_view;
    MouseGraphic* mouseGraphic = m_mouseGraphic;
    Controller* controller = m_controller;

    // The thread on which the controller will execute
    m_mouseAlgoThread = new QThread();
    QThread* thread = m_mouseAlgoThread;

    // We need to actually spawn the QProcess (i.e., m_process = new
    // QProcess()) in the separate thread, hence why this is async. Note that
    // we need the separate thread because, while it's performing an
    // algorithm-requested action, the Controller could block the GUI loop from
    // executing.
    connect(m_mouseAlgoThread, &QThread::started, m_controller, [=](){
        // We need to add the mouse to the world *after* the the controller is
        // initialized (thus ensuring that tile fog is cleared automatically),
        // but *before* we actually start the algorithm (lest the mouse
        // position/orientation not be updated properly during the beginning of
        // the mouse algo's execution)
        controller->init();
        Model::get()->addMouse("", mouse);
        controller->start(name);
    });

    connect(thread, &QThread::finished, this, [=](){
        controller->deleteLater();
        thread->deleteLater();
        // TODO: MACK - if we remove the wait(), the
        // following delete statements cause problems
        delete mouseGraphic;
        delete view;
        delete mouse;
        // qDebug() << "THREAD DEAD";
    });
    m_controller->moveToThread(m_mouseAlgoThread);
	m_mouseAlgoThread->start();
}

void Window::stopMouseAlgo() {
    m_map.setView(m_truth);
    if (m_controller != nullptr) {
        m_controller = nullptr;
        m_map.setMouseGraphic(nullptr);
        m_mouseAlgoThread->quit();
        // TODO: MACK - necessary for the thread to actually get killed
        m_mouseAlgoThread->wait();
        // TODO: MACK - we need some synchronization here, because the model
        // could be using the pointer at the time it's updated
        Model::get()->removeMouse("");
    }
}

#if(0)
void Window::keyPress(int key) {

    // NOTE: If you're adding or removing anything from this function, make
    // sure to update wiki/Keys.md

    if (key == Qt::Key_F) {
        // Faster (only in discrete mode)
        if (m_controller != nullptr) {
            if (m_controller->getInterfaceType(false) == InterfaceType::DISCRETE) {
                S()->setSimSpeed(S()->simSpeed() * 1.5);
            }
            else {
                qWarning().noquote().nospace()
                    << "Increasing the simulator speed is only allowed in "
                    << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                    << " mode.";
            }
        }
    }
    else if (key == Qt::Key_S) {
        // Slower (only in discrete mode)
        if (m_controller != nullptr) {
            if (m_controller->getInterfaceType(false) == InterfaceType::DISCRETE) {
                S()->setSimSpeed(S()->simSpeed() / 1.5);
            }
            else {
                qWarning().noquote().nospace()
                    << "Decreasing the simulator speed is only allowed in "
                    << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                    << " mode.";
            }
        }
    }
    else if (key == Qt::Key_L) {
        // Cycle through the available layouts
        S()->setLayoutType(LAYOUT_TYPE_CYCLE.value(S()->layoutType()));
    }
    else if (key == Qt::Key_R) {
        // Toggle rotate zoomed map
        S()->setRotateZoomedMap(!S()->rotateZoomedMap());
    }
    else if (key == Qt::Key_I) {
        // Zoom in
        S()->setZoomedMapScale(S()->zoomedMapScale() * 1.5);
    }
    else if (key == Qt::Key_O) {
        // Zoom out
        S()->setZoomedMapScale(S()->zoomedMapScale() / 1.5);
    }
    else if (key == Qt::Key_T) {
        // Toggle wall truth visibility
        S()->setWallTruthVisible(!S()->wallTruthVisible());
        // TODO: MACK - update both MazeViews?
        m_view->getMazeGraphic()->updateWalls();
    }
    else if (key == Qt::Key_C) {
        // Toggle tile colors
        S()->setTileColorsVisible(!S()->tileColorsVisible());
        // TODO: MACK - update both MazeViews?
        m_view->getMazeGraphic()->updateColor();
    }
    else if (key == Qt::Key_G) {
        // Toggle tile fog
        S()->setTileFogVisible(!S()->tileFogVisible());
        // TODO: MACK - update both MazeViews?
        m_view->getMazeGraphic()->updateFog();
    }
    else if (key == Qt::Key_X) {
        // Toggle tile text
        S()->setTileTextVisible(!S()->tileTextVisible());
        // TODO: MACK - update both MazeViews?
        m_view->getMazeGraphic()->updateText();
    }
    else if (key == Qt::Key_D) {
        // Toggle tile distance visibility
        S()->setTileDistanceVisible(!S()->tileDistanceVisible());
        // TODO: MACK - update both MazeViews?
        m_view->getMazeGraphic()->updateText();
    }
    else if (key == Qt::Key_Q) {
        // Quit
        SimUtilities::quit();
    }
    else if (
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
    else if (
        key == Qt::Key_Up || key == Qt::Key_Down ||
        key == Qt::Key_Left || key == Qt::Key_Right
    ) {
        S()->setArrowKeyIsPressed(key, true);
    }
}

void Window::keyRelease(int key) {
    if (
        key == Qt::Key_Up || key == Qt::Key_Down ||
        key == Qt::Key_Left || key == Qt::Key_Right
    ) {
        S()->setArrowKeyIsPressed(key, false);
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
    if (Model::get()->containsMouse("")) {
        stats = Model::get()->getMouseStats("");
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
        {"Elapsed Real Time", SimUtilities::formatDuration(Time::get()->elapsedRealTime())},
        {"Elapsed Sim Time", SimUtilities::formatDuration(Time::get()->elapsedSimTime())},
        {"Time Since Origin Departure",
            stats.timeOfOriginDeparture.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(
                Time::get()->elapsedSimTime() - stats.timeOfOriginDeparture)
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
#endif

} // namespace mms
