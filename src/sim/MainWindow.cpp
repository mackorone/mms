#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QtCore>

#include "Assert.h"
#include "Map.h"
#include "MazeFileType.h"
#include "Model.h"
#include "Param.h"
#include "ParamParser.h"
#include "ProcessUtilities.h"
#include "SettingsMazeAlgos.h"
#include "SettingsMazeFiles.h"
#include "SettingsMouseAlgos.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"

namespace mms {

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        m_maze(nullptr),
        m_truth(nullptr),
        m_mouse(nullptr),
        m_mouseGraphic(nullptr),
        m_view(nullptr),
        m_controller(nullptr) {

    // Initialize the UI
    ui.setupUi(this);

    // Add map to the UI
    ui.mapLayout->addWidget(&m_map);

    // Resize the window
    resize(P()->defaultWindowWidth(), P()->defaultWindowHeight());

    // TODO: MACK - settings somewhere
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(10);
    ui.mazeBuildTextEdit->document()->setDefaultFont(font);
    ui.mazeRunTextEdit->document()->setDefaultFont(font);
    ui.buildTextEdit->document()->setDefaultFont(font);
    ui.runTextEdit->document()->setDefaultFont(font);
    ui.mazeBuildTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    ui.mazeRunTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    ui.buildTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    ui.runTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    // Load saved algos
    for (const QString& algoName : SettingsMazeAlgos::names()) {
        ui.selectMazeAlgorithmComboBox->addItem(algoName);
    }
    for (const QString& algoName : SettingsMouseAlgos::names()) {
        ui.selectAlgorithmComboBox->addItem(algoName);
    }

    // Connect the maze algo buttons
    connect(ui.importMazeAlgoButton, &QPushButton::clicked, this,&MainWindow::importMazeAlgo);
    connect(ui.editMazeAlgoButton, &QPushButton::clicked, this, &MainWindow::editMazeAlgo);
    connect(ui.buildMazeAlgoButton, &QPushButton::clicked, this, &MainWindow::buildMazeAlgo);
    connect(ui.runMazeAlgoButton, &QPushButton::clicked, this, &MainWindow::runMazeAlgo);

    // Connect the mouse algo buttons
    connect(ui.importMouseAlgoButton, &QPushButton::clicked, this, &MainWindow::importMouseAlgo);
    connect(ui.editMouseAlgoButton, &QPushButton::clicked, this, &MainWindow::editMouseAlgo);
    connect(ui.buildButton, &QPushButton::clicked, this, &MainWindow::buildMouseAlgo);
    connect(ui.runButton, &QPushButton::clicked, this, &MainWindow::runMouseAlgo);

    // TODO: MACK
    connect(ui.editParametersButton, &QPushButton::clicked, this, [](){
        ParamParser::execEditDialog();
    });

    // TODO: MACK - connect the import mazes button
    connect(
        ui.importMazesButton,
        &QPushButton::clicked,
        this,
        [&](){
            QStringList suffixes;
            for (const QString& suffix : MAZE_FILE_TYPE_TO_SUFFIX.values()) {
                suffixes.append(QString("*.") + suffix);
            }
            QStringList paths = QFileDialog::getOpenFileNames(
                this,
                tr("Select one or more maze files to import"),
                "/home",
                QString("Mazes Files (") + suffixes.join(" ") + ")"
            );
            for (const QString& path : paths) {
                SettingsMazeFiles::addMazeFile(path);
            }
            refreshSettingsMazeFiles();
        }
    );

    // TODO: MACK - set some maze files table properties
    ui.mazeFilesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.mazeFilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.mazeFilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.mazeFilesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); 
    refreshSettingsMazeFiles();

    connect(
        ui.mazeFilesTable,
        &QTableWidget::itemSelectionChanged,
        this,
        [&](){
            QString path = ui.mazeFilesTable->selectedItems().at(1)->text();
            Maze* maze = Maze::fromFile(path);
            if (maze != nullptr) {
                setMaze(maze);
            }
        }
    );

    // Connect pause button
    connect(ui.pauseButton, &QPushButton::clicked, this, &MainWindow::togglePause);
}

MainWindow::~MainWindow() {
    // TODO: MACK - other cleanup here
    delete m_maze;
}

void MainWindow::keyPress(int key) {

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

void MainWindow::keyRelease(int key) {
    if (
        key == Qt::Key_Up || key == Qt::Key_Down ||
        key == Qt::Key_Left || key == Qt::Key_Right
    ) {
        S()->setArrowKeyIsPressed(key, false);
    }
}

void MainWindow::setMaze(Maze* maze) {
    Maze* prev_maze = m_maze;
    MazeView* prev_truth = m_truth;

    m_maze = maze;
    m_truth = new MazeView(m_maze);
    m_map.setMaze(m_maze);

    bool respawn = m_controller != nullptr;
    stopMouseAlgo();

    Model::get()->setMaze(m_maze);
    delete prev_maze;
    delete prev_truth;

    if (respawn) {
        runMouseAlgo();
    }
}

void MainWindow::togglePause() {
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

void MainWindow::refreshSettingsMazeFiles() {
    QStringList mazeFiles = SettingsMazeFiles::getSettingsMazeFiles();
    ui.mazeFilesTable->setRowCount(mazeFiles.size());
    ui.mazeFilesTable->setColumnCount(2);
    for (int i = 0; i < mazeFiles.size(); i += 1) {
        QString path = mazeFiles.at(i);
        ui.mazeFilesTable->setItem(i, 0, new QTableWidgetItem(QFileInfo(path).fileName()));
        ui.mazeFilesTable->setItem(i, 1, new QTableWidgetItem(path));
    }
}

// TODO: MACK
void MainWindow::importMazeAlgo() {
    QString name = SettingsMazeAlgos::execImportDialog();
    if (!name.isEmpty()) {
        // TODO: MACK - resort, select the updated algo
        ui.selectMazeAlgorithmComboBox->addItem(name);
        int index = ui.selectMazeAlgorithmComboBox->findText(name);
        ui.selectMazeAlgorithmComboBox->setCurrentIndex(index);
    }
}

void MainWindow::editMazeAlgo() {

    // TODO: MACK - cancelling causes combo box to change selection

    QString name = ui.selectMazeAlgorithmComboBox->currentText();
    ASSERT_TR(SettingsMazeAlgos::names().contains(name));

    QString newName = SettingsMazeAlgos::execEditDialog(name);
    ui.selectMazeAlgorithmComboBox->clear();
    for (const QString& algoName : SettingsMazeAlgos::names()) {
        ui.selectMazeAlgorithmComboBox->addItem(algoName);
    }
    if (!newName.isEmpty()) {
        // TODO: MACK - resort, select the updated algo
        int index = ui.selectMazeAlgorithmComboBox->findText(newName);
        ui.selectMazeAlgorithmComboBox->setCurrentIndex(index);
    }
}

void MainWindow::buildMazeAlgo() {

    // Determine the maze to build
    const QString& mazeAlgoName = ui.selectMazeAlgorithmComboBox->currentText();
    ASSERT_TR(SettingsMazeAlgos::names().contains(mazeAlgoName));
    // TODO: MACK - check settings actually contains these values and contains
    // and path validity here
    QString dirPath = SettingsMazeAlgos::getDirPath(mazeAlgoName);
    QString mazeBuildCommand = SettingsMazeAlgos::getBuildCommand(mazeAlgoName);
    qDebug() << "COMMAND:" << mazeBuildCommand;

    // First, instantiate a new process
    m_buildMazeAlgoProcess = new QProcess(this);
    m_buildMazeAlgoProcess->setWorkingDirectory(dirPath);

    // Listen for build errors
    connect(
        m_buildMazeAlgoProcess,
        &QProcess::readyReadStandardError,
        this,
        [&](){
            QString errors = m_buildMazeAlgoProcess->readAllStandardError();
            ui.mazeBuildTextEdit->appendPlainText(errors);
        }
    );

    // TODO: MACK
    connect(
        m_buildMazeAlgoProcess,
        &QProcess::started,
        this,
        [=](){
            qDebug() << "STARTED";
        }
    );

    // Re-enable build button when build finishes, clean up the process
    connect(
        m_buildMazeAlgoProcess,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        [&](int exitCode, QProcess::ExitStatus exitStatus){
            ui.buildMazeAlgoButton->setEnabled(true);
            m_buildMazeAlgoProcess->deleteLater();
        }
    );
            
    // Disable the build button before build starts
    ui.buildMazeAlgoButton->setEnabled(false);

    // Now, start the build
    bool success = ProcessUtilities::start(m_buildMazeAlgoProcess, mazeBuildCommand);
    if (!success) {
        // TODO: MACK
        qDebug() << "Failed to run:" << mazeBuildCommand;
        ui.buildMazeAlgoButton->setEnabled(true);
        m_buildMazeAlgoProcess->deleteLater();
    }
}

void MainWindow::runMazeAlgo() {

    const QString& mazeAlgoName = ui.selectMazeAlgorithmComboBox->currentText();
    ASSERT_TR(SettingsMazeAlgos::names().contains(mazeAlgoName));

    // TODO: MACK - check settings actually contains these values and contains
    // and path validity here
    QString dirPath = SettingsMazeAlgos::getDirPath(mazeAlgoName);
    QString mazeRunCommand = SettingsMazeAlgos::getRunCommand(mazeAlgoName);

    // Add the height and width to the command
    mazeRunCommand += " ";
    mazeRunCommand += ui.widthSpinBox->cleanText();
    mazeRunCommand += " ";
    mazeRunCommand += ui.heightSpinBox->cleanText();

    // First, instantiate a new process
    m_runMazeAlgoProcess = new QProcess(this);
    m_runMazeAlgoProcess->setWorkingDirectory(dirPath);

    // Listen for output
    connect(
        m_runMazeAlgoProcess,
        &QProcess::readyReadStandardOutput,
        this,
        [&](){
            QString output = m_runMazeAlgoProcess->readAllStandardOutput();
            ui.mazeRunTextEdit->appendPlainText(output);
        }
    );

    // Re-enable build button when build finishes
    connect(
        m_runMazeAlgoProcess,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        [&](int exitCode, QProcess::ExitStatus exitStatus){
            ui.runMazeAlgoButton->setEnabled(true);
            QString output = m_runMazeAlgoProcess->readAllStandardError();
            Maze* maze = Maze::fromAlgo(output.toUtf8());
            if (maze != nullptr) {
                setMaze(maze);
            }
            m_runMazeAlgoProcess->deleteLater();
        }
    );
            
    // Disable the build button before build starts
    ui.runMazeAlgoButton->setEnabled(false);

    // Now, start the build
    bool success = ProcessUtilities::start(m_runMazeAlgoProcess, mazeRunCommand);
    if (!success) {
            // TODO: MACK
            qDebug() << "Failed to run:" << mazeRunCommand;
            ui.runMazeAlgoButton->setEnabled(true);
            m_runMazeAlgoProcess->deleteLater();
    }
}

void MainWindow::importMouseAlgo() {
    QString name = SettingsMouseAlgos::execImportDialog();
    if (!name.isEmpty()) {
        // TODO: MACK - resort, select the updated algo
        ui.selectAlgorithmComboBox->addItem(name);
        int index = ui.selectAlgorithmComboBox->findText(name);
        ui.selectAlgorithmComboBox->setCurrentIndex(index);
    }
}

void MainWindow::editMouseAlgo() {

    QString name = ui.selectAlgorithmComboBox->currentText();
    ASSERT_TR(SettingsMouseAlgos::names().contains(name));

    QString newName = SettingsMouseAlgos::execEditDialog(name);
    ui.selectAlgorithmComboBox->clear();
    for (const QString& algoName : SettingsMouseAlgos::names()) {
        ui.selectAlgorithmComboBox->addItem(algoName);
    }
    if (!newName.isEmpty()) {
        // TODO: MACK - resort, select the updated algo
        int index = ui.selectAlgorithmComboBox->findText(newName);
        ui.selectAlgorithmComboBox->setCurrentIndex(index);
    }
}

void MainWindow::buildMouseAlgo() {

    // TODO: MACK - helper for this (ensure all fields exist)
    const QString& algoName = ui.selectAlgorithmComboBox->currentText();
    ASSERT_TR(SettingsMouseAlgos::names().contains(algoName));

    // TODO: MACK - check settings actually contains these values and contains
    // and path validity here
    QString dirPath = SettingsMouseAlgos::getDirPath(algoName);
    QString buildCommand = SettingsMouseAlgos::getBuildCommand(algoName);

    // First, instantiate a new process
    m_buildMouseAlgoProcess = new QProcess(this);
    m_buildMouseAlgoProcess->setWorkingDirectory(dirPath);

    // Listen for build errors
    connect(
        m_buildMouseAlgoProcess,
        &QProcess::readyReadStandardError,
        this,
        [&](){
            QString errors = m_buildMouseAlgoProcess->readAllStandardError();
            ui.buildTextEdit->appendPlainText(errors);
        }
    );

    // Re-enable build button when build finishes, clean up the process
    connect(
        m_buildMouseAlgoProcess,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        [&](int exitCode, QProcess::ExitStatus exitStatus){
            ui.buildButton->setEnabled(true);
            m_buildMouseAlgoProcess->deleteLater();
        }
    );
            
    // Disable the build button before build starts
    ui.buildButton->setEnabled(false);

    // Now, start the build
    ProcessUtilities::start(m_buildMouseAlgoProcess, buildCommand);
}

void MainWindow::runMouseAlgo() {

    // TODO: MACK - respawn after switching the mouse algo combo box selection

    const QString& algoName = ui.selectAlgorithmComboBox->currentText();
    ASSERT_TR(SettingsMouseAlgos::names().contains(algoName));

    // Generate the mouse, lens, and controller
    m_mouse = new Mouse(m_maze);
    QString mouseFilePath = SettingsMouseAlgos::getMouseFilePath(algoName);
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
        ui.runTextEdit,
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

    Mouse* mouse = m_mouse;
    MazeViewMutable* view = m_view;
    MouseGraphic* mouseGraphic = m_mouseGraphic;
    Controller* controller = m_controller;

    // The thread on which the controller will execute
    m_runMouseAlgoThread = new QThread();
    QThread* thread = m_runMouseAlgoThread;

    // We need to actually spawn the QProcess (i.e., m_process = new
    // QProcess()) in the separate thread, hence why this is async. Note that
    // we need the separate thread because, while it's performing an
    // algorithm-requested action, the Controller could block the GUI loop from
    // executing.
    connect(m_runMouseAlgoThread, &QThread::started, m_controller, [=](){
        // We need to add the mouse to the world *after* the the controller is
        // initialized (thus ensuring that tile fog is cleared automatically),
        // but *before* we actually start the algorithm (lest the mouse
        // position/orientation not be updated properly during the beginning of
        // the mouse algo's execution)
        controller->init();
        Model::get()->addMouse("", mouse);
        controller->start(algoName);
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
    m_controller->moveToThread(m_runMouseAlgoThread);
	m_runMouseAlgoThread->start();
}

void MainWindow::stopMouseAlgo() {
    m_map.setView(m_truth);
    if (m_controller != nullptr) {
        m_controller = nullptr;
        m_map.setMouseGraphic(nullptr);
        m_runMouseAlgoThread->quit();
        // TODO: MACK - necessary for the thread to actually get killed
        m_runMouseAlgoThread->wait();
        // TODO: MACK - we need some synchronization here, because the model
        // could be using the pointer at the time it's updated
        Model::get()->removeMouse("");
    }
}

QVector<QPair<QString, QVariant>> MainWindow::getRunStats() const {

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

QVector<QPair<QString, QVariant>> MainWindow::getAlgoOptions() const {
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

QVector<QPair<QString, QVariant>> MainWindow::getMazeInfo() const {
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

QVector<QPair<QString, QVariant>> MainWindow::getOptions() const {
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

} // namespace mms
