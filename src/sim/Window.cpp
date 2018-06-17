#include "Window.h"

#include <QAction>
#include <QFileDialog>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLinkedList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>

#include "ColorManager.h"
#include "ConfigDialog.h"
#include "MazeFilesTab.h"
#include "Model.h"
#include "Param.h"
#include "ProcessUtilities.h"
#include "Resources.h"
#include "SettingsMouseAlgos.h"
#include "SettingsMisc.h"
#include "SimTime.h"
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
        m_maze(nullptr),
        m_truth(nullptr),
        m_mouse(nullptr),
        m_mouseGraphic(nullptr),
        m_view(nullptr),
        m_runInterface(nullptr),

        // MouseAlgosTab
        m_mouseAlgoWidget(new QWidget()),
        m_mouseAlgoComboBox(new QComboBox()),
        m_mouseAlgoEditButton(new QPushButton("Edit")),
        m_mouseAlgoImportButton(new QPushButton("Import")),
        m_buildProcess(nullptr),
        m_buildButton(new QPushButton("Build")),
        m_buildStatus(new QLabel()),
        m_buildOutput(new QPlainTextEdit()),
        m_runProcess(nullptr),
        m_runButton(new QPushButton("Run")),
        m_runStatus(new QLabel()),
        m_runOutput(new QPlainTextEdit()),
        m_mouseAlgoStatsWidget(new MouseAlgoStatsWidget()),
        m_mouseAlgoSeedWidget(new RandomSeedWidget()),
        m_mouseAlgoPauseButton(new QPushButton("Pause")) {

    // First, some bookkeeping; we have to explicitly allow the
    // mouse process exit event to be handled on the GUI thread
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

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

    // Set the default values for the map options
    m_truthButton->setChecked(true);
    m_viewButton->setEnabled(false);

	// Add functionality to the buttons
    connect(m_viewButton, &QRadioButton::toggled, this, [=](bool checked){
        m_map.setView(checked ? m_view : m_truth);
    });

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

    // Create the mouse algos tab
    mouseAlgoTabInit();
    tabWidget->addTab(m_mouseAlgoWidget, "Mouse Algorithms");

    // Add some generic menu items
    QMenu* fileMenu = menuBar()->addMenu(tr("&Menu"));

    // Settings
    QAction* settingsAction = new QAction(tr("&Settings"), this);
    connect(settingsAction, &QAction::triggered, this, &Window::editSettings);
    fileMenu->addAction(settingsAction);

    // Quit
    QAction* quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, this, &Window::close);
    fileMenu->addAction(quitAction);

    // Save the maze
    // TODO: MACK - select the maze type here...
    /*
    QAction* saveMazeAction = new QAction(tr("&Save Maze As ..."), this);
    connect(saveMazeAction, &QAction::triggered, this, [=](){
        QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save File"),
            "",
            tr("Images (*.png *.xpm *.jpg)")
        );
        qDebug() << filename;
    });
    fileMenu->addAction(saveMazeAction);
    */

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
    resize(
        SettingsMisc::getRecentWindowWidth(),
        SettingsMisc::getRecentWindowHeight());

    // Start the graphics loop
    // TODO: upforgrabs
    // Make fps configurable
    double secondsPerFrame = 1.0 / 60;
    QTimer* mapTimer = new QTimer();
    connect(
        mapTimer, &QTimer::timeout,
        this, [=](){
            // TODO: MACK - this makes the graphics look choppy
            // - fix this to only run when dialog is open
            // Hack to prevent file dialog from locking up...
            static double then = SimUtilities::getHighResTimestamp();
            double now = SimUtilities::getHighResTimestamp();
            if (now - then < secondsPerFrame) {
                return;
            }
            m_map.update();
            //m_model.step();
            then = now;
        }
    );
    mapTimer->start(secondsPerFrame * 1000);

    // TODO: MACK - this is very expensive - fix it
    /*
    // Start the info loop
    QTimer* otherTimer = new QTimer();
    QLinkedList<double>* timestamps = new QLinkedList<double>();
    int numFrames = 10;
    connect(otherTimer, &QTimer::timeout, this, [=](){
        double now = SimUtilities::getHighResTimestamp();
        timestamps->append(now);
        if (timestamps->size() > numFrames) {
            double then = timestamps->takeFirst();
            double fps = numFrames / (now - then);
        }
        QPair<QStringList, QVector<QVariant>> runStats = getRunStats();
        QStringList keys = runStats.first;
        QVector<QVariant> values = runStats.second;
        for (int i = 0; i < keys.size(); i += 1) {
            QString text = values.at(i).toString();
            if (values.at(i).type() == QVariant::Double) {
                text = QString::number(values.at(i).toDouble(), 'f', 3);
            }
            m_runStats.value(keys.at(i))->setText(text);
        }
    });
    //otherTimer->start(100); // 10 fps
    */
}

/*
void Window::modelStep() {
    static double DT
    static double prev = SimUtilities::getHighResTimestamp();
    static double acc = 0.0;
    double now = SimUtilities::getHighResTimestamp();
    acc += (now - prev) * 1.0; // m_simSpeed; TODO: MACK
    prev = now;
    while (acc >= DT) {
        update(DT);
        acc -= DT;
        // TODO: MACK - check for collisions ...
        // std::thread collisionDetector(&Model::checkCollision, this);
    }
}

void Window::update(double dt) {

    // Ensure the maze/mouse aren't updated in this loop
    m_mutex.lock();

    // If there's nothing to update, sleep for a little bit
    if (m_mouse == nullptr || m_paused) {
        m_mutex.unlock();
        return;
    }

    // Calculate the amount of sim time that should pass during this iteration
    Duration elapsedSimTimeForThisIteration = Duration::Seconds(dt);

    // Update the sim time
    SimTime::get()->incrementElapsedSimTime(elapsedSimTimeForThisIteration);

    // Update the position of the mouse
    m_mouse->update(elapsedSimTimeForThisIteration);

    // Retrieve the current discretized location of the mouse
    QPair<int, int> location = m_mouse->getCurrentDiscretizedTranslation();

    // If we're ever outside of the maze, crash. It would be cool to have
    // some "out of bounds" state but I haven't implemented that yet. We
    // continue here to make sure that we join with the other thread.
    if (!m_maze->withinMaze(location.first, location.second)) {
        m_mouse->setCrashed();
        m_mutex.unlock();
        return;
    }

    // Retrieve the tile at current location
    const Tile* tileAtLocation = m_maze->getTile(location.first, location.second);

    // If this is a new tile, update the set of traversed tiles
    if (!m_stats->traversedTileLocations.contains(location)) {
        m_stats->traversedTileLocations.insert(location);
        if (m_stats->closestDistanceToCenter == -1 ||
                tileAtLocation->getDistance() < m_stats->closestDistanceToCenter) {
            m_stats->closestDistanceToCenter = tileAtLocation->getDistance(); 
        }
        // Alert any listeners that a new tile was entered
        emit newTileLocationTraversed(location.first, location.second);
    }

    // If we've returned to the origin, reset the departure time
    if (location.first == 0 && location.second == 0) {
        m_stats->timeOfOriginDeparture = Duration::Seconds(-1);
    }

    // Otherwise, if we've just left the origin, update the departure time
    else if (m_stats->timeOfOriginDeparture < Duration::Seconds(0)) {
        m_stats->timeOfOriginDeparture = SimTime::get()->elapsedSimTime();
    }

    // Separately, if we're in the center, update the best time to center
    if (m_maze->isCenterTile(location.first, location.second)) {
        Duration timeToCenter = SimTime::get()->elapsedSimTime() - m_stats->timeOfOriginDeparture;
        if (
            m_stats->bestTimeToCenter < Duration::Seconds(0) ||
            timeToCenter < m_stats->bestTimeToCenter
        ) {
            m_stats->bestTimeToCenter = timeToCenter;
        }
    }

    // Release the mutex
    m_mutex.unlock();
}
*/

void Window::resizeEvent(QResizeEvent* event) {
    SettingsMisc::setRecentWindowWidth(event->size().width());
    SettingsMisc::setRecentWindowHeight(event->size().height());
}

void Window::closeEvent(QCloseEvent *event) {
    // Graceful shutdown
    cancelBuild();
    cancelRun();
    m_map.shutdown();
    QMainWindow::closeEvent(event);
}

void Window::setMaze(Maze* maze) {

    // Stop running maze/mouse algos
    // TODO: MACK -stop the build too?
    cancelRun();

    // Next, update the maze and truth
    Maze* oldMaze = m_maze;
    MazeView* oldTruth = m_truth;
    m_maze = maze;
    m_truth = new MazeView(m_maze);

    // The truth has walls declared and distance as text
    MazeGraphic* mazeGraphic = m_truth->getMazeGraphic();
    for (int x = 0; x < m_maze->getWidth(); x += 1) {
        for (int y = 0; y < m_maze->getHeight(); y += 1) {
            const Tile* tile = m_maze->getTile(x, y);
            for (Direction d : DIRECTIONS()) {
                mazeGraphic->declareWall(x, y, d, tile->isWall(d));
            }
            int distance = tile->getDistance();
            mazeGraphic->setTileText(x, y, 0 <= distance
                ? QString::number(distance)
                : "inf"
            );
        }
    }

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
        DIRECTION_TO_STRING().value(m_maze->getOptimalStartingDirection())
    );
    m_isValidLabel->setText(m_maze->isValidMaze() ? "TRUE" : "FALSE");
    m_isOfficialLabel->setText(m_maze->isOfficialMaze() ? "TRUE" : "FALSE");

    // Delete the old objects
    delete oldMaze;
    delete oldTruth;
}

void Window::editSettings() {

    ConfigDialog dialog(
        "Edit",
        "Settings",
        {
        },
        false // No "Remove" button
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
}

QPair<QStringList, QVector<QVariant>> Window::getRunStats() const {

    static QStringList keys = {
        "Tiles Traversed",
        "Closest Distance to Center",
        "Current X (m)",
        "Current Y (m)",
        "Current Rotation (deg)",
        "Current X Tile",
        "Current Y Tile",
        "Current Direction",
        "Elapsed Real Time",
        "Elapsed Sim Time",
        "Time Since Origin Departure",
        "Best Time to Center",
        "Crashed",
    };

    QVector<QVariant> values;
    MouseStats stats = m_model.getMouseStats();

    // This means the mouse isn't in the maze
    if (stats.closestDistanceToCenter < 0) {
        for (int i = 0; i < keys.size(); i += 1) {
            values.append("N/A");
        }
    }
    else {
        // TODO: MACK - m_mouse can be null here :/ ...
        values.append(
            QString::number(stats.traversedTileLocations.size()) + " / " +
            QString::number(m_maze->getWidth() * m_maze->getHeight())
        );
        values.append(stats.closestDistanceToCenter);
        values.append(m_mouse->getCurrentTranslation().getX().getMeters());
        values.append(m_mouse->getCurrentTranslation().getY().getMeters());
        values.append(m_mouse->getCurrentRotation().getDegreesZeroTo360());
        values.append(m_mouse->getCurrentDiscretizedTranslation().first);
        values.append(m_mouse->getCurrentDiscretizedTranslation().second);
        values.append(DIRECTION_TO_STRING().value(m_mouse->getCurrentDiscretizedRotation()));
        values.append(SimUtilities::formatDuration(SimTime::get()->elapsedRealTime()));
        values.append(SimUtilities::formatDuration(SimTime::get()->elapsedSimTime()));
        values.append(
            stats.timeOfOriginDeparture.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(
                SimTime::get()->elapsedSimTime() - stats.timeOfOriginDeparture)
        );
        values.append(
            stats.bestTimeToCenter.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(stats.bestTimeToCenter)
        );
        values.append((m_mouse->didCrash() ? "TRUE" : "FALSE"));
    }

    return {keys, values};
}

#if(0)
QVector<QPair<QString, QVariant>> Window::getAlgoOptions() const {
    return {
        // Mouse Info
        // TODO: MACK - interface type not finalized
        {"Interface Type",
            m_runInterface == nullptr
            ? "NONE"
            : INTERFACE_TYPE_TO_STRING().value(m_runInterface->getInterfaceType(false))
        },
        /*
        QString("Initial Direction:           ") + (m_runInterface == nullptr ? "NONE" :
            m_runInterface->getStaticOptions().initialDirection),
        QString("Tile Text Num Rows:          ") + (m_runInterface == nullptr ? "NONE" :
            QString::number(m_runInterface->getStaticOptions().tileTextNumberOfRows)),
        QString("Tile Text Num Cols:          ") + (m_runInterface == nullptr ? "NONE" :
            QString::number(m_runInterface->getStaticOptions().tileTextNumberOfCols)),
        */
        {"Allow Omniscience",
            m_runInterface == nullptr
            ? "NONE"
            : m_runInterface->getDynamicOptions().allowOmniscience ? "TRUE" : "FALSE"
        },
        {"Declare Both Wall Halves",
            m_runInterface == nullptr
            ? "NONE"
            : m_runInterface->getDynamicOptions().declareBothWallHalves ? "TRUE" : "FALSE"
        },
        {"Auto Set Tile Text",
            m_runInterface == nullptr
            ? "NONE"
            : m_runInterface->getDynamicOptions().setTileTextWhenDistanceDeclared ? "TRUE" : "FALSE"
        },
        {"Auto Set Tile Base Color",
            m_runInterface == nullptr
            ? "NONE"
            : m_runInterface->getDynamicOptions().setTileBaseColorWhenDistanceDeclaredCorrectly ? "TRUE" : "FALSE"
        }
        // TODO: MACK
        /*
        QString("Wheel Speed Fraction:        ") +
            (m_runInterface == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE().value(m_runInterface->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            QString::number(m_runInterface->getStaticOptions().wheelSpeedFraction))),
        QString("Declare Wall On Read:        ") +
            (m_runInterface == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE().value(m_runInterface->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_runInterface->getDynamicOptions().declareWallOnRead ? "TRUE" : "FALSE"))),
        QString("Use Tile Edge Movements:     ") +
            (m_runInterface == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE().value(m_runInterface->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_runInterface->getDynamicOptions().useTileEdgeMovements ? "TRUE" : "FALSE"))),
        */
    };
}
#endif

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
        m_buildButton, &QPushButton::clicked,
        this, &Window::startBuild
    );
    connect(
        m_runButton, &QPushButton::clicked,
        this, &Window::startRun
    );

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
    algorithmLayout->addWidget(m_mouseAlgoComboBox, 0, 0, 1, 2);
    algorithmLayout->addWidget(m_mouseAlgoImportButton, 1, 0);
    algorithmLayout->addWidget(m_mouseAlgoEditButton, 1, 1);
    connect(
        m_mouseAlgoComboBox, &QComboBox::currentTextChanged,
        this, [=](QString name){
            SettingsMisc::setRecentMouseAlgo(name);
        }
    );

    // Actions groupbox
    QGroupBox* actionsGroupBox = new QGroupBox("Actions");
    topLayout->addWidget(actionsGroupBox, 1, 0);
    QGridLayout* actionsLayout = new QGridLayout();
    actionsGroupBox->setLayout(actionsLayout);
    actionsLayout->addWidget(m_buildButton, 0, 0);
    actionsLayout->addWidget(m_buildStatus, 0, 1);
    actionsLayout->addWidget(m_runButton, 1, 0);
    actionsLayout->addWidget(m_runStatus, 1, 1);
    for (QLabel* label : {m_buildStatus, m_runStatus}) {
        label->setAlignment(Qt::AlignCenter);   
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        label->setMinimumWidth(90);
    }

    // Options group box
    QGroupBox* optionsGroupBox = new QGroupBox("Options");
    topLayout->addWidget(optionsGroupBox, 0, 2, 2, 2);
    QVBoxLayout* optionsLayout = new QVBoxLayout();
    optionsGroupBox->setLayout(optionsLayout);

    // Add the random seed widget
    optionsLayout->addWidget(m_mouseAlgoSeedWidget);

    // Runtime controls group box
    QGroupBox* controlsGroupBox = new QGroupBox("Controls");
    optionsLayout->addWidget(controlsGroupBox, 0, 0);
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlsGroupBox->setLayout(controlLayout);

    // Add the pause button and speed slider
    QHBoxLayout* speedsLayout = new QHBoxLayout();
    controlLayout->addLayout(speedsLayout);
    double maxSpeed = 10.0; // 10x normal speed
    QSlider* speedSlider = new QSlider(Qt::Horizontal);
    QDoubleSpinBox* speedBox = new QDoubleSpinBox();
    speedBox->setRange(maxSpeed / 100.0, maxSpeed);
    speedsLayout->addWidget(new QLabel("Speed"));
    speedsLayout->addWidget(speedSlider);
    speedsLayout->addWidget(speedBox);
    speedsLayout->addWidget(m_mouseAlgoPauseButton);

    // Set up the pause button initial state
    m_mouseAlgoPauseButton->setEnabled(false);
    mouseAlgoResume();

    // Add the logic for the speed slider
    connect(
        speedSlider, &QSlider::valueChanged,
        this, [=](int value){
            double fraction = (value + 1.0) / 100.0;
            speedBox->setValue(fraction * maxSpeed);
            m_model.setSimSpeed(fraction * maxSpeed);
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
        button->setEnabled(false);
        button->setMinimumSize(3, 0);
        inputButtonsLayout->addWidget(button);
        m_mouseAlgoInputButtons.append(button);
    }
    controlLayout->addLayout(inputButtonsLayout);

    // Add the build and run output
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    layout->addLayout(bottomLayout);
    m_mouseAlgoOutputTabWidget = new QTabWidget();
    bottomLayout->addWidget(m_mouseAlgoOutputTabWidget);
    m_mouseAlgoOutputTabWidget->addTab(m_buildOutput, "Build Output");
    m_mouseAlgoOutputTabWidget->addTab(m_runOutput, "Run Output");
    m_mouseAlgoOutputTabWidget->addTab(m_runOutput, "Run Output");
    m_mouseAlgoOutputTabWidget->addTab(m_mouseAlgoStatsWidget, "Stats");

    // Set the default values for some widgets
    for (QPlainTextEdit* output : {
        m_buildOutput,
        m_runOutput,
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

    // Add the algo run stats
    QPair<QStringList, QVector<QVariant>> runStats = getRunStats();
    m_mouseAlgoStatsWidget->init(runStats.first);

    // Add the mouse algos
    mouseAlgoRefresh(SettingsMisc::getRecentMouseAlgo());
}

void Window::mouseAlgoEdit() {

    QString name = m_mouseAlgoComboBox->currentText();

    QVector<ConfigDialogField> fields = mouseAlgoGetFields();
    ConfigDialogField nameField = fields.at(0);
    ConfigDialogField dirPathField = fields.at(1);
    ConfigDialogField buildCommandField = fields.at(2);
    ConfigDialogField runCommandField = fields.at(3);

    nameField.initialLineEditValue = name;
    dirPathField.initialLineEditValue = SettingsMouseAlgos::getDirPath(name);
    buildCommandField.initialLineEditValue =
        SettingsMouseAlgos::getBuildCommand(name);
    runCommandField.initialLineEditValue =
        SettingsMouseAlgos::getRunCommand(name);

    ConfigDialog dialog(
        "Edit",
        "Mouse Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField,
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
    QString newName = dialog.getLineEditValue(nameField.label);
    SettingsMouseAlgos::update(
        name,
        newName,
        dialog.getLineEditValue(dirPathField.label),
        dialog.getLineEditValue(buildCommandField.label),
        dialog.getLineEditValue(runCommandField.label)
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

    ConfigDialog dialog(
        "Import",
        "Mouse Algorithm",
        {
            nameField,
            dirPathField,
            buildCommandField,
            runCommandField,
        },
        false // No "Remove" button
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Ok was pressed
    QString name = dialog.getLineEditValue(nameField.label);
    SettingsMouseAlgos::add(
        name,
        dialog.getLineEditValue(dirPathField.label),
        dialog.getLineEditValue(buildCommandField.label),
        dialog.getLineEditValue(runCommandField.label)
    );

    // Update the mouse algos
    mouseAlgoRefresh(name);
}

void Window::startBuild() {

    // Only one build at a time
    ASSERT_TR(m_buildProcess == nullptr);

    // Get the command and directory
    QString algoName = m_mouseAlgoComboBox->currentText();
    QString command = SettingsMouseAlgos::getBuildCommand(algoName);
    QString dirPath = SettingsMouseAlgos::getDirPath(algoName);

    // Validation
    if (command.isEmpty()) {
        QMessageBox::warning(
            this,
            QString("Empty Build Command"),
            QString("Build command for algorithm \"%1\" is empty.").arg(
                m_mouseAlgoComboBox->currentText()
            )
        );
        return;
    }
    if (dirPath.isEmpty()) {
        QMessageBox::warning(
            this,
            QString("Empty Directory"),
            QString("Directory for algorithm \"%1\" is empty.").arg(
                m_mouseAlgoComboBox->currentText()
            )
        );
        return;
    }

    // Instantiate a new process
    QProcess* process = new QProcess(this);

    // Display build stdout and stderr
    connect(process, &QProcess::readyReadStandardOutput, this, [=](){
        QString output = process->readAllStandardOutput();
        if (output.endsWith("\n")) {
            output.truncate(output.size() - 1);
        }
        m_buildOutput->appendPlainText(output);
    });
    connect(process, &QProcess::readyReadStandardError, this, [=](){
        QString output = process->readAllStandardError();
        if (output.endsWith("\n")) {
            output.truncate(output.size() - 1);
        }
        m_buildOutput->appendPlainText(output);
    });

    // Clean up on exit
    connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        &Window::onBuildExit
    );

    // Clean the ouput and bring it to the front
    m_buildOutput->clear();
    m_mouseAlgoOutputTabWidget->setCurrentWidget(m_buildOutput);

    // Start the build process
    if (ProcessUtilities::start(command, dirPath, process)) {

        // Save a pointer to the process
        m_buildProcess = process;

        // Update the build button
        disconnect(
            m_buildButton,
            &QPushButton::clicked,
            this, &Window::startBuild);
        connect(
            m_buildButton,
            &QPushButton::clicked,
            this, &Window::cancelBuild);
        m_buildButton->setText("Cancel");

        // Update the build status
        m_buildStatus->setText("BUILDING");
        m_buildStatus->setStyleSheet(
            "QLabel { background: rgb(255, 255, 100); }"
        );
    }
    else {
        // Clean up the failed process
        m_buildOutput->appendPlainText(process->errorString());
        m_buildStatus->setText("ERROR");
        m_buildStatus->setStyleSheet(
            "QLabel { background: rgb(230, 150, 230); }"
        );
        delete process;
    }
}

void Window::onBuildExit(int exitCode, QProcess::ExitStatus exitStatus) {

    // Update the build button
    disconnect(
        m_buildButton,
        &QPushButton::clicked,
        this, &Window::cancelBuild);
    connect(
        m_buildButton,
        &QPushButton::clicked,
        this, &Window::startBuild);
    m_buildButton->setText("Build");

    // Update the build status
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        m_buildStatus->setText("COMPLETE");
        m_buildStatus->setStyleSheet(
            "QLabel { background: rgb(150, 255, 100); }"
        );
    }
    else {
        m_buildStatus->setText("FAILED");
        m_buildStatus->setStyleSheet(
            "QLabel { background: rgb(255, 150, 150); }"
        );
    }

    // Clean up the build process
    delete m_buildProcess;
    m_buildProcess = nullptr;
}

void Window::startRun() {

    // Only one algo running at a time
    ASSERT_TR(m_runProcess == nullptr);

    // Get the command and directory
    QString algoName = m_mouseAlgoComboBox->currentText();
    QString dirPath = SettingsMouseAlgos::getDirPath(algoName);
    QString command = SettingsMouseAlgos::getRunCommand(algoName);

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

    // Validate the maze
    if (m_maze == nullptr) {
        QMessageBox::warning(
            this,
            "No Maze",
            "You must load a maze before running a mouse algorithm."
        );
        return;
    }
    if (!m_maze->isValidMaze()) {
        QMessageBox::warning(
            this,
            "Invalid Maze",
            "Cannot run mouse algorithm because the maze is invalid. The maze "
            "must be nonempty, rectangular, enclosed, and self-consistent."
        );
        return;
    }

    // TODO: MACK - delete the font files too

    // Remove the mouse from the maze
    removeMouseFromMaze();

    // Generate the mouse
    m_mouse = new Mouse(m_maze);
    m_view = new MazeView(m_maze);
    m_mouseGraphic = new MouseGraphic(m_mouse);

    // New interface and process
    MouseInterface* interface = new MouseInterface(
        m_maze,
        m_mouse,
        m_view
    );
    QProcess* process = new QProcess();

    // Clear the output, and jump to it
    m_runOutput->clear();
    m_mouseAlgoOutputTabWidget->setCurrentWidget(m_runOutput);

    // Append the random seed to the command
    command += " ";
    command += QString::number(m_mouseAlgoSeedWidget->next());

    // Print stdout
    connect(
        process,
        &QProcess::readyReadStandardOutput,
        interface,
        [=](){
            QString output = process->readAllStandardOutput();
            if (output.endsWith("\n")) {
                output.truncate(output.size() - 1);
            }
            m_runOutput->appendPlainText(output);
        }
    );

    // Process all stderr commands as appropriate
    connect(
        process,
        &QProcess::readyReadStandardError,
        // Handle the process's stderr on the mouse's event loop to
        // prevent the UI from freezing during a blocking mouse action
        interface,
        [=](){
            QString text = process->readAllStandardError();
            QStringList lines = getLines(text, &m_commandBuffer);
            for (const QString& line : lines) {
                QString response = interface->dispatch(line);
                if (!response.isEmpty()) {
                    process->write((response + "\n").toStdString().c_str());
                }
            }
        }
    );

    // Connect the input buttons to the algorithm
    for (int i = 0; i < m_mouseAlgoInputButtons.size(); i += 1) {
        QPushButton* button = m_mouseAlgoInputButtons.at(i);
        connect(button, &QPushButton::clicked, this, [=](){
            button->setEnabled(false);
            // Note: we can't call inputButtonWasPressed directly because
            // it's not thread-safe; instead, we use a queued connection
            emit inputButtonWasPressed(i);
        });
        connect(
            this,
            &Window::inputButtonWasPressed,
            interface,
            &MouseInterface::inputButtonWasPressed
        );
        connect(
            interface,
            &MouseInterface::inputButtonWasAcknowledged,
            this,
            [=](int button) {
                m_mouseAlgoInputButtons.at(button)->setEnabled(true);
            }
        );
    }

    // We need to add the mouse to the world *after* the making the
    // previous connection (thus ensuring that tile fog is cleared
    // automatically), but *before* we actually start the algorithm (lest
    // the mouse position/orientation not be updated properly during the
    // beginning of the mouse algo's execution)
    m_model.setMouse(m_mouse);

    // Clean up on exit
    connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        &Window::onRunExit
    );

    // Start the run process
    if (ProcessUtilities::start(command, dirPath, process)) {

        m_runInterface = interface;
        m_runProcess = process;
        m_map.setView(m_view);
        m_map.setMouseGraphic(m_mouseGraphic);

        // Update the run button
        disconnect(
            m_runButton,
            &QPushButton::clicked,
            this, &Window::startRun);
        connect(
            m_runButton,
            &QPushButton::clicked,
            this, &Window::cancelRun);
        m_runButton->setText("Cancel");

        // Update the run status
        m_runStatus->setText("RUNNING");
        m_runStatus->setStyleSheet(
            "QLabel { background: rgb(255, 255, 100); }"
        );

        // TODO: MACK - most of these shouldn't be necessary
        // UI updates on successful start
        m_viewButton->setEnabled(true);
        m_viewButton->setChecked(true);
        m_mouseAlgoPauseButton->setEnabled(true);
        for (QPushButton* button : m_mouseAlgoInputButtons) {
            button->setEnabled(true);
        }
    } 
    else {
        // Clean up the failed process
        m_runOutput->appendPlainText(process->errorString());
        m_runStatus->setText("ERROR");
        m_runStatus->setStyleSheet(
            "QLabel { background: rgb(230, 150, 230); }"
        );
        removeMouseFromMaze();
        delete interface;
        delete process;
    }
}

void Window::onRunExit(int exitCode, QProcess::ExitStatus exitStatus) {

    // TODO: MACK - deduplicate this with onBuildExit

    // Update the run button
    disconnect(
        m_runButton,
        &QPushButton::clicked,
        this, &Window::cancelRun);
    connect(
        m_runButton,
        &QPushButton::clicked,
        this, &Window::startRun);
    m_runButton->setText("Run");

    // Update the status label, call stderrPostAction
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        m_runStatus->setText("COMPLETE");
        m_runStatus->setStyleSheet(
            "QLabel { background: rgb(150, 255, 100); }"
        );
    }
    else {
        m_runStatus->setText("FAILED");
        m_runStatus->setStyleSheet(
            "QLabel { background: rgb(255, 150, 150); }"
        );
    }

    // Clean up
    delete m_runProcess;
    m_runProcess = nullptr;
    delete m_runInterface;
    m_runInterface = nullptr;
    m_commandBuffer.clear();
}

void Window::removeMouseFromMaze() {

    // No-op if no mouse
    if (m_mouse == nullptr) {
        return;
    }

    // Update some objects
    m_model.removeMouse();
    m_map.setView(m_truth);
    m_map.setMouseGraphic(nullptr);

    // Delete some objects
    ASSERT_FA(m_view == nullptr);
    ASSERT_FA(m_mouseGraphic == nullptr);
    delete m_mouse;
    m_mouse = nullptr;
    delete m_view;
    m_view = nullptr;
    delete m_mouseGraphic;
    m_mouseGraphic = nullptr;

    // TODO: MACK - most of these shouldn't be necessary
    // Clean up the UI
    m_truthButton->setChecked(true);
    m_viewButton->setEnabled(false);
    m_mouseAlgoPauseButton->setEnabled(false);
    mouseAlgoResume();
    for (QPushButton* button : m_mouseAlgoInputButtons) {
        button->setEnabled(false);
    }
}

void Window::cancelBuild() {
    cancelProcess(m_buildProcess, m_buildStatus);
}

void Window::cancelRun() {
    cancelProcess(m_runProcess, m_runStatus);
    removeMouseFromMaze();
}

void Window::cancelProcess(QProcess* process, QLabel* status) {
    if (process == nullptr) {
        return;
    }
    process->kill();
    process->waitForFinished();
    status->setText("CANCELED");
    status->setStyleSheet(
        "QLabel { background: rgb(180, 180, 180); }"
    );
}

void Window::handleMouseAlgoCannotStart(QString errorString) {
    m_runStatus->setText("ERROR");
    m_runStatus->setStyleSheet(
        "QLabel { background: rgb(255, 150, 150); }"
    );
    m_runOutput->appendPlainText(errorString);
    m_model.removeMouse();
}


void Window::mouseAlgoPause() {
    m_model.setPaused(true);
    m_mouseAlgoPauseButton->setText("Resume");
    disconnect(
        m_mouseAlgoPauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoPause
    );
    connect(
        m_mouseAlgoPauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoResume
    );
}

void Window::mouseAlgoResume() {
    m_model.setPaused(false);
    m_mouseAlgoPauseButton->setText("Pause");
    disconnect(
        m_mouseAlgoPauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoResume
    );
    connect(
        m_mouseAlgoPauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoPause
    );
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
    m_buildButton->setEnabled(!isEmpty);
    m_runButton->setEnabled(!isEmpty);
}

QVector<ConfigDialogField> Window::mouseAlgoGetFields() {

    ConfigDialogField nameField;
    nameField.label = "Name";
    nameField.type = ConfigDialogFieldType::STRING;
    nameField.allowEmptyLineEditValue = false;

    ConfigDialogField dirPathField;
    dirPathField.label = "Directory";
    dirPathField.type = ConfigDialogFieldType::DIRECTORY;

    ConfigDialogField buildCommandField;
    buildCommandField.label = "Build Command";
    buildCommandField.type = ConfigDialogFieldType::STRING;

    ConfigDialogField runCommandField;
    runCommandField.label = "Run Command";
    runCommandField.type = ConfigDialogFieldType::STRING;

    return {
        nameField,
        dirPathField,
        buildCommandField,
        runCommandField,
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
