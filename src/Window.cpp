#include "Window.h"

#include <QAction>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLinkedList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QShortcut>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

#include "AssertMacros.h"
#include "Color.h"
#include "ColorDialog.h"
#include "ColorManager.h"
#include "ConfigDialog.h"
#include "Dimensions.h"
#include "FontImage.h"
#include "ProcessUtilities.h"
#include "SettingsMazeFiles.h"
#include "SettingsMouseAlgos.h"
#include "SettingsMisc.h"
#include "SimUtilities.h"

namespace mms {

const QString Window::IN_PROGRESS_STYLE_SHEET =
    "QLabel { background: rgb(255, 255, 100); }";
const QString Window::COMPLETE_STYLE_SHEET =
    "QLabel { background: rgb(150, 255, 100); }";
const QString Window::CANCELED_STYLE_SHEET =
    "QLabel { background: rgb(180, 180, 180); }";
const QString Window::FAILED_STYLE_SHEET =
    "QLabel { background: rgb(255, 150, 150); }";
const QString Window::ERROR_STYLE_SHEET =
    "QLabel { background: rgb(230, 150, 230); }";

const QString Window::ACK = "ack";
const QString Window::CRASH = "crash";
const QString Window::INVALID = "invalid";

const int Window::SPEED_SLIDER_MAX = 99;
const int Window::SPEED_SLIDER_DEFAULT = 33;
const double Window::PROGRESS_REQUIRED_FOR_MOVE = 100.0;
const double Window::PROGRESS_REQUIRED_FOR_TURN = 33.33;
const double Window::MIN_PROGRESS_PER_SECOND = 10.0;
const double Window::MAX_PROGRESS_PER_SECOND = 5000.0;
const double Window::MAX_SLEEP_SECONDS = 0.008;

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    m_map(new Map()),

    // Maze
    m_maze(nullptr),
    m_truth(nullptr),
    m_currentMazeFile(QString()),
    m_mazeFileComboBox(new QComboBox()),

    // Algo config
    m_mouseAlgoComboBox(new QComboBox()),
    m_mouseAlgoEditButton(new QToolButton()),

    // Algo output
    m_mouseAlgoOutputTabWidget(new QTabWidget()),
    m_buildOutput(new QPlainTextEdit()),
    m_runOutput(new QPlainTextEdit()),

    // Algo build
    m_buildButton(new QPushButton("Build")),
    m_buildProcess(nullptr),
    m_buildStatus(new QLabel()),

    // Algo run
    m_runButton(new QPushButton("Run")),
    m_runProcess(nullptr),
    m_runStatus(new QLabel()),
    m_mouse(nullptr),
    m_view(nullptr),
    m_mouseGraphic(nullptr),

    // Pause/reset
    m_isPaused(false),
    m_wasReset(false),
    m_pauseButton(new QPushButton("Pause")),
    m_resetButton(new QPushButton("Reset")),

    // Communication
    m_logBuffer(QStringList()),
    m_commandBuffer(QStringList()),
    m_commandQueue(QQueue<QString>()),
    m_commandQueueTimer(new QTimer()),

    // Movement
    m_startingLocation({0, 0}),
    m_startingDirection(Direction::NORTH),
    m_movement(Movement::NONE),
    m_doomedToCrash(false),
    m_movesRemaining(0),
    m_movementProgress(0.0),
    m_movementStepSize(0.0),
    m_speedSlider(new QSlider(Qt::Horizontal)),

    // Helpers
    m_tilesWithColor(QSet<QPair<int, int>>()),
    m_tilesWithText(QSet<QPair<int, int>>()) {

    // Keyboard shortcuts for closing the window
    QShortcut* ctrl_q = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);
    QShortcut* ctrl_w = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this);
    connect(ctrl_q, &QShortcut::activated, this, &QMainWindow::close);
    connect(ctrl_w, &QShortcut::activated, this, &QMainWindow::close);

    // Add the map and panel to the window
    QVBoxLayout* panelLayout = new QVBoxLayout();
    panelLayout->setContentsMargins(0, 6, 6, 6);
    QWidget* panel = new QWidget();
    panel->setLayout(panelLayout);
    QSplitter* splitter = new QSplitter();
    splitter->setHandleWidth(9);
    splitter->addWidget(m_map);
    splitter->addWidget(panel);
    setCentralWidget(splitter);

    // Add the upper parts of the panel
    QGroupBox* configGroupBox = new QGroupBox("Config");
    QGridLayout* configLayout = new QGridLayout();
    configGroupBox->setLayout(configLayout);
    QGroupBox* controlsGroupBox = new QGroupBox("Controls");
    QGridLayout* controlsLayout = new QGridLayout();
    controlsGroupBox->setLayout(controlsLayout);
    QHBoxLayout* upperLayout = new QHBoxLayout();
    upperLayout->addWidget(controlsGroupBox);
    upperLayout->addWidget(configGroupBox);
    panelLayout->addLayout(upperLayout);
    QWidget* statsWidget = new QWidget();
    QGridLayout* statsLayout = new QGridLayout();
    statsWidget->setLayout(statsLayout);

    // Add the mouse algo build and run buttons
    controlsLayout->addWidget(m_buildButton, 0, 0);
    controlsLayout->addWidget(m_runButton, 1, 0);
    connect(m_buildButton, &QPushButton::clicked, this, &Window::startBuild);
    connect(m_runButton, &QPushButton::clicked, this, &Window::startRun);

    // Add the mouse algo build and run statuses
    controlsLayout->addWidget(m_buildStatus, 0, 1);
    controlsLayout->addWidget(m_runStatus, 1, 1);
    for (QLabel* label : {m_buildStatus, m_runStatus}) {
        label->setAlignment(Qt::AlignCenter);   
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        label->setMinimumWidth(90);
    }

    // Add mouse algo pause and reset buttons
    m_pauseButton->setEnabled(false);
    m_resetButton->setEnabled(false);
    controlsLayout->addWidget(m_pauseButton, 0, 2, 1, 1);
    controlsLayout->addWidget(m_resetButton, 0, 3, 1, 1);
    connect(
        m_pauseButton,
        &QPushButton::clicked,
        this,
        &Window::onPauseButtonPressed
    );
    connect(
        m_resetButton,
        &QPushButton::pressed,
        this,
        &Window::onResetButtonPressed
    ); 

    // Add mouse algo speed
    QLabel* turtle = new QLabel();
    QLabel* rabbit = new QLabel();
    turtle->setPixmap(QPixmap(":/resources/icons/turtle.png"));
    rabbit->setPixmap(QPixmap(":/resources/icons/rabbit.png"));
    QHBoxLayout* speedLayout = new QHBoxLayout();
    speedLayout->addWidget(turtle);
    speedLayout->addWidget(m_speedSlider);
    speedLayout->addWidget(rabbit);
    controlsLayout->addLayout(speedLayout, 1, 2, 1, 2);
    m_speedSlider->setRange(0, SPEED_SLIDER_MAX);
    m_speedSlider->setValue(SPEED_SLIDER_DEFAULT);

    // Add config box labels
    QLabel* mazeLabel = new QLabel("Maze");
    QLabel* mouseLabel = new QLabel("Mouse");
    configLayout->addWidget(mazeLabel, 0, 0, 1, 1);
    configLayout->addWidget(mouseLabel, 1, 0, 1, 1);
    QSizePolicy policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mazeLabel->setSizePolicy(policy);
    mouseLabel->setSizePolicy(policy);

    // Add maze file combo box
    m_mazeFileComboBox->setMinimumContentsLength(1);
    configLayout->addWidget(m_mazeFileComboBox, 0, 1, 1, 2);
    connect(
        m_mazeFileComboBox,
        static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::activated),
        this,
        &Window::onMazeFileComboBoxChanged
    );

    // Add color dialog button
    QToolButton* colorButton = new QToolButton();
    colorButton->setIcon(QIcon(":/resources/icons/color.png"));
    configLayout->addWidget(colorButton, 0, 4, 1, 1);
    connect(
        colorButton,
        &QPushButton::clicked,
        this,
        &Window::onColorButtonPressed
    );

    // Add mouse algo combo box
    m_mouseAlgoComboBox->setMinimumContentsLength(1);
    configLayout->addWidget(m_mouseAlgoComboBox, 1, 1, 1, 2);
    connect(
        m_mouseAlgoComboBox,
        &QComboBox::currentTextChanged,
        this,
        &Window::onMouseAlgoComboBoxChanged
    );

    // Add maze file load button
    QToolButton* mazeFileOpenButton = new QToolButton();
    configLayout->addWidget(mazeFileOpenButton, 0, 3, 1, 1);
    mazeFileOpenButton->setIcon(QIcon(":/resources/icons/open.png"));
    connect(
        mazeFileOpenButton,
        &QToolButton::clicked,
        this, 
        &Window::onMazeFileButtonPressed
    );

    // Add mouse algo edit button
    configLayout->addWidget(m_mouseAlgoEditButton, 1, 3, 1, 1);
    m_mouseAlgoEditButton->setIcon(QIcon(":/resources/icons/edit.png"));
    connect(
        m_mouseAlgoEditButton,
        &QPushButton::clicked,
        this,
        &Window::onMouseAlgoEditButtonPressed
    );

    // Add mouse algo import button
    QToolButton* mouseAlgoImportButton = new QToolButton();
    configLayout->addWidget(mouseAlgoImportButton, 1, 4, 1, 1);
    mouseAlgoImportButton->setIcon(QIcon(":/resources/icons/plus.png"));
    connect(
        mouseAlgoImportButton,
        &QPushButton::clicked,
        this,
        &Window::onMouseAlgoImportButtonPressed
    );

    // Add stats labels
    stats = new Stats();
    createStat("Total Distance", StatsEnum::TOTAL_DISTANCE, 0, 0, 0, 1, statsLayout);
    createStat("Total Effective Distance", StatsEnum::TOTAL_EFFECTIVE_DISTANCE, 1, 0, 1, 1, statsLayout);
    createStat("Total Turns", StatsEnum::TOTAL_TURNS, 2, 0, 2, 1, statsLayout);
    createStat("Current Run Distance", StatsEnum::CURRENT_RUN_DISTANCE, 0, 2, 0, 3, statsLayout);
    createStat("Current Run Effective Distance", StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE, 1, 2, 1, 3, statsLayout);
    createStat("Current Run Turns", StatsEnum::CURRENT_RUN_TURNS, 2, 2, 2, 3, statsLayout);
    createStat("Best Run Distance", StatsEnum::BEST_RUN_DISTANCE, 3, 0, 3, 1, statsLayout);
    createStat("Best Run Effective Distance", StatsEnum::BEST_RUN_EFFECTIVE_DISTANCE, 4, 0, 4, 1, statsLayout);
    createStat("Best Run Turns", StatsEnum::BEST_RUN_TURNS, 5, 0, 5, 1, statsLayout);
    createStat("Score", StatsEnum::SCORE, 6, 0, 6, 1, statsLayout);

    // Add the build and run outputs to the panel
    panelLayout->addWidget(m_mouseAlgoOutputTabWidget);
    m_mouseAlgoOutputTabWidget->addTab(m_buildOutput, "Build Output");
    m_mouseAlgoOutputTabWidget->addTab(m_runOutput, "Run Output");
    m_mouseAlgoOutputTabWidget->addTab(statsWidget, "Stats");
    for (QPlainTextEdit* output : {m_buildOutput, m_runOutput}) {
        output->setReadOnly(true);
        output->setLineWrapMode(QPlainTextEdit::NoWrap);
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSize(10);
        output->document()->setDefaultFont(font);
    }

    // Resize the window and make the map square
    int windowWidth = SettingsMisc::getRecentWindowWidth();
    int windowHeight = SettingsMisc::getRecentWindowHeight();
    resize(windowWidth, windowHeight);
    splitter->setSizes({windowHeight, windowWidth - windowHeight});

    // Remove maze files that no longer exist
    for (const auto& path : SettingsMazeFiles::getAllPaths()) {
        if (path.isEmpty() || !QFileInfo::exists(path)) {
            SettingsMazeFiles::removePath(path);
        }
    }

    // Load the recently used maze
    QString path = SettingsMisc::getRecentMazeFile();
    Maze* maze = Maze::fromFile(path);
    if (maze == nullptr) {
        path = ":/resources/mazes/blank.num";
        maze = Maze::fromFile(path);
    }
    refreshMazeFileComboBox(path);
    updateMazeAndPath(maze, path);

    // Add the mouse algos
    refreshMouseAlgoComboBox(SettingsMisc::getRecentMouseAlgo());

    // Configure command queue timer
    m_commandQueueTimer->setSingleShot(true);
    connect(
        m_commandQueueTimer,
        &QTimer::timeout,
        this,
        &Window::processQueuedCommands
    );

    // Start the graphics loop
    double secondsPerFrame = 1.0 / 60;
    QTimer* mapTimer = new QTimer();
    connect(
        mapTimer, &QTimer::timeout,
        this, [=](){
            // Hack to prevent file dialog from locking up...
            static double then = SimUtilities::getHighResTimestamp();
            double now = SimUtilities::getHighResTimestamp();
            if (now - then < secondsPerFrame) {
                return;
            }
            m_map->update();
            then = now;
        }
    );
    mapTimer->start(secondsPerFrame * 1000);
}

void Window::resizeEvent(QResizeEvent* event) {
    SettingsMisc::setRecentWindowWidth(event->size().width());
    SettingsMisc::setRecentWindowHeight(event->size().height());
}

void Window::closeEvent(QCloseEvent *event) {
    cancelAllProcesses();
    m_map->shutdown();
    QMainWindow::closeEvent(event);
}

void Window::onMazeFileButtonPressed() {
    QString path = QFileDialog::getOpenFileName(this, tr("Load Maze"));
    if (path.isNull()) {
        return;
    }
    Maze* maze = Maze::fromFile(path);
    if (maze == nullptr) {
        showInvalidMazeFileWarning(path);
        return;
    }
    SettingsMazeFiles::addPath(path);
    refreshMazeFileComboBox(path);
    updateMazeAndPath(maze, path);
}

void Window::onMazeFileComboBoxChanged(QString path) {
    Maze* maze = Maze::fromFile(path);
    if (maze == nullptr) {
        refreshMazeFileComboBox(m_currentMazeFile);
        showInvalidMazeFileWarning(path);
        return;
    }
    updateMazeAndPath(maze, path);
    stats->resetAll();
}

void Window::onColorButtonPressed() {
    ColorDialog dialog(
        CHAR_TO_COLOR().key(ColorManager::get()->getTileBaseColor()),
        CHAR_TO_COLOR().key(ColorManager::get()->getTileWallColor()),
        CHAR_TO_COLOR().key(ColorManager::get()->getMouseBodyColor()),
        CHAR_TO_COLOR().key(ColorManager::get()->getMouseWheelColor()),
        CHAR_TO_COLOR().key(ColorManager::get()->getTileWallIsSetColor()),
        ColorManager::get()->getTileWallNotSetAlpha()
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Update the settings (disk and in-memory cache)
    ColorManager::get()->update(
        CHAR_TO_COLOR().value(dialog.getTileBaseColor()),
        CHAR_TO_COLOR().value(dialog.getTileWallColor()),
        CHAR_TO_COLOR().value(dialog.getMouseBodyColor()),
        CHAR_TO_COLOR().value(dialog.getMouseWheelColor()),
        CHAR_TO_COLOR().value(dialog.getTileWallIsSetColor()),
        dialog.getTileWallNotSetAlpha()
    );

    // Redraw the "truth" view with the new colors
    m_truth->getMazeGraphic()->refreshColors();

    // Redraw the mouse's view with the new colors
    if (m_view != nullptr) {
        m_view->getMazeGraphic()->refreshColors();
    }
}

void Window::showInvalidMazeFileWarning(QString path) {
    QMessageBox::warning(
        this,
        "Invalid Maze File",
        "The following is not a valid maze file:\n\n" + path + "\n\n "
        "The maze must be nonempty, rectangular, enclosed, and consistent."
    );
}

void Window::refreshMazeFileComboBox(QString selected) {
    m_mazeFileComboBox->clear();
    for (const auto& info : QDir(":/resources/mazes/").entryInfoList()) {
        m_mazeFileComboBox->addItem(info.absoluteFilePath());
    }
    for (const auto& path : SettingsMazeFiles::getAllPaths()) {
        m_mazeFileComboBox->addItem(path);
    }
    m_mazeFileComboBox->setCurrentText(selected);
}

void Window::updateMazeAndPath(Maze* maze, QString path) {
    updateMaze(maze);
    m_currentMazeFile = path;
    SettingsMisc::setRecentMazeFile(path);
}

void Window::updateMaze(Maze* maze) {

    // Stop running maze/mouse algos
    cancelAllProcesses();

    // Next, update the maze and truth
    Maze* oldMaze = m_maze;
    MazeView* oldTruth = m_truth;
    m_maze = maze;
    m_truth = new MazeView(m_maze, true);

    // The truth has walls declared and distance as text
    MazeGraphic* mazeGraphic = m_truth->getMazeGraphic();
    for (int x = 0; x < m_maze->getWidth(); x += 1) {
        for (int y = 0; y < m_maze->getHeight(); y += 1) {
            const Tile* tile = m_maze->getTile(x, y);
            for (Direction d : DIRECTIONS()) {
                if (tile->isWall(d)) {
                    mazeGraphic->setWall(x, y, d);
                }
            }
            int distance = tile->getDistance();
            QString text = 0 <= distance ? QString::number(distance) : "inf";
            mazeGraphic->setText(x, y, text);
        }
    }

    // Update pointers held by other objects
    m_map->setMaze(m_maze);
    m_map->setView(m_truth);

    // Delete the old objects
    delete oldMaze;
    delete oldTruth;
}

void Window::onMouseAlgoComboBoxChanged(QString name) {
    cancelAllProcesses();
    m_buildStatus->setText("");
    m_buildStatus->setStyleSheet("");
    m_buildOutput->clear();
    m_runStatus->setText("");
    m_runStatus->setStyleSheet("");
    m_runOutput->clear();
    stats->resetAll();
    SettingsMisc::setRecentMouseAlgo(name);
}

void Window::onMouseAlgoEditButtonPressed() {

    // Create the dialog with initial values
    QString name = m_mouseAlgoComboBox->currentText();
    ConfigDialog dialog(
        name,
        SettingsMouseAlgos::getDirectory(name),
        SettingsMouseAlgos::getBuildCommand(name),
        SettingsMouseAlgos::getRunCommand(name)
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Remove was pressed
    if (dialog.removeButtonPressed()) {
        SettingsMouseAlgos::remove(name);
        refreshMouseAlgoComboBox("");
        return;
    }

    // OK was pressed
    QString newName = dialog.getName();
    SettingsMouseAlgos::update(
        name,
        newName,
        dialog.getDirectory(),
        dialog.getBuildCommand(),
        dialog.getRunCommand()
    );

    // Update the mouse algos
    refreshMouseAlgoComboBox(newName);
}

void Window::onMouseAlgoImportButtonPressed() {

    // Create an empty dialog
    ConfigDialog dialog("", "", "", "");

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Ok was pressed
    QString newName = dialog.getName();
    SettingsMouseAlgos::add(
        newName,
        dialog.getDirectory(),
        dialog.getBuildCommand(),
        dialog.getRunCommand()
    );

    // Update the mouse algos
    refreshMouseAlgoComboBox(newName);
}

void Window::refreshMouseAlgoComboBox(QString selected) {
    m_mouseAlgoComboBox->clear();
    for (const auto& name : SettingsMouseAlgos::names()) {
        m_mouseAlgoComboBox->addItem(name);
    }
    m_mouseAlgoComboBox->setCurrentText(selected);
    bool isNonempty = m_mouseAlgoComboBox->count();
    m_mouseAlgoComboBox->setEnabled(isNonempty);
    m_mouseAlgoEditButton->setEnabled(isNonempty);
    m_buildButton->setEnabled(isNonempty);
    m_runButton->setEnabled(isNonempty);
}

void Window::cancelProcess(QProcess* process, QLabel* status) {
    if (process == nullptr) {
        return;
    }
    process->kill();
    process->waitForFinished();
    status->setText("CANCELED");
    status->setStyleSheet(CANCELED_STYLE_SHEET);
}

void Window::cancelAllProcesses() {
    cancelBuild();
    cancelRun();
}

void Window::startBuild() {

    // Only one build at a time
    ASSERT_TR(m_buildProcess == nullptr);

    // Extract the relevant config
    QString name = m_mouseAlgoComboBox->currentText();
    QString directory = SettingsMouseAlgos::getDirectory(name);
    QString buildCommand = SettingsMouseAlgos::getBuildCommand(name);

    // Validation
    if (directory.isEmpty()) {
        QMessageBox::warning(
            this,
            QString("Empty Directory"),
            QString("Directory for \"%1\" is empty.").arg(
                m_mouseAlgoComboBox->currentText()
            )
        );
        return;
    }
    if (buildCommand.isEmpty()) {
        QMessageBox::warning(
            this,
            QString("Empty Build Command"),
            QString("Build command for \"%1\" is empty.").arg(
                m_mouseAlgoComboBox->currentText()
            )
        );
        return;
    }

    // Instantiate a new process
    QProcess* process = new QProcess(this);

    // Display stdout and stderr
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

    // Clear the ouput and bring it to the front
    m_buildOutput->clear();
    m_mouseAlgoOutputTabWidget->setCurrentWidget(m_buildOutput);

    // Start the build process
    if (ProcessUtilities::start(buildCommand, directory, process)) {

        // Save a pointer to the process
        m_buildProcess = process;

        // Update the build button
        disconnect(
            m_buildButton,
            &QPushButton::clicked,
            this,
            &Window::startBuild
        );
        connect(
            m_buildButton,
            &QPushButton::clicked,
            this,
            &Window::cancelBuild
        );
        m_buildButton->setText("Cancel");

        // Update the build status
        m_buildStatus->setText("BUILDING");
        m_buildStatus->setStyleSheet(IN_PROGRESS_STYLE_SHEET);
    }
    else {
        // Clean up the failed process
        m_buildOutput->appendPlainText(process->errorString());
        m_buildStatus->setText("ERROR");
        m_buildStatus->setStyleSheet(ERROR_STYLE_SHEET);
        delete process;
    }
}

void Window::cancelBuild() {
    cancelProcess(m_buildProcess, m_buildStatus);
}

void Window::onBuildExit(int exitCode, QProcess::ExitStatus exitStatus) {

    // Update the build button
    disconnect(
        m_buildButton,
        &QPushButton::clicked,
        this,
        &Window::cancelBuild
    );
    connect(
        m_buildButton,
        &QPushButton::clicked,
        this,
        &Window::startBuild
    );
    m_buildButton->setText("Build");

    // Update the build status
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        m_buildStatus->setText("COMPLETE");
        m_buildStatus->setStyleSheet(COMPLETE_STYLE_SHEET);
    }
    else {
        m_buildStatus->setText("FAILED");
        m_buildStatus->setStyleSheet(FAILED_STYLE_SHEET);
    }

    // Clean up
    delete m_buildProcess;
    m_buildProcess = nullptr;
}

void Window::startRun() {

    // Only one algo running at a time
    ASSERT_TR(m_runProcess == nullptr);

    // Extract the relevant config
    QString name = m_mouseAlgoComboBox->currentText();
    QString directory = SettingsMouseAlgos::getDirectory(name);
    QString runCommand = SettingsMouseAlgos::getRunCommand(name);

    // Validation
    if (directory.isEmpty()) {
        QMessageBox::warning(
            this,
            "Empty Directory",
            QString("Directory for \"%1\" is empty.").arg(
                name
            )
        );
        return;
    }
    if (runCommand.isEmpty()) {
        QMessageBox::warning(
            this,
            "Empty Run Command",
            QString("Run command for \"%1\" is empty.").arg(
                name
            )
        );
        return;
    }
    ASSERT_FA(m_maze == nullptr);

    // Remove the old mouse, add a new mouse
    removeMouseFromMaze();
    m_mouse = new Mouse();
    m_view = new MazeView(m_maze, false);
    m_mouseGraphic = new MouseGraphic(m_mouse);
    m_map->setView(m_view);
    m_map->setMouseGraphic(m_mouseGraphic);

    // Instantiate a new process
    QProcess* process = new QProcess();

    // Print stderr
    connect(process, &QProcess::readyReadStandardError, this, [=](){
        QString output = process->readAllStandardError();
        QStringList logs = processText(output, &m_logBuffer);
        for (QString log : logs) {
            m_runOutput->appendPlainText(log);
        }
    });

    // Process commands from stdout
    connect(process, &QProcess::readyReadStandardOutput, this, [=](){
        QString output = process->readAllStandardOutput();
        QStringList commands = processText(output, &m_commandBuffer);
        for (QString command : commands) {
            dispatchCommand(command);
        }
    });

    // Clean up on exit
    connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        this,
        &Window::onRunExit
    );

    // Clear the ouput and bring it to the front
    m_runOutput->clear();
    m_mouseAlgoOutputTabWidget->setCurrentWidget(m_runOutput);

    // reset score
    stats->resetAll();

    // Start the run process
    if (ProcessUtilities::start(runCommand, directory, process)) {

        // Save a pointer to the process
        m_runProcess = process;

        // Update the run button
        disconnect(
            m_runButton,
            &QPushButton::clicked,
            this,
            &Window::startRun
        );
        connect(
            m_runButton,
            &QPushButton::clicked,
            this,
            &Window::cancelRun
        );
        m_runButton->setText("Cancel");

        // Update the run status
        m_runStatus->setText("RUNNING");
        m_runStatus->setStyleSheet(IN_PROGRESS_STYLE_SHEET);

        // Only enabled while mouse is running
        m_pauseButton->setEnabled(true);
        m_resetButton->setEnabled(true);
    } 
    else {
        // Clean up the failed process
        m_runOutput->appendPlainText(process->errorString());
        m_runStatus->setText("ERROR");
        m_runStatus->setStyleSheet(ERROR_STYLE_SHEET);
        removeMouseFromMaze();
        delete process;
    }
}

void Window::cancelRun() {
    cancelProcess(m_runProcess, m_runStatus);
    removeMouseFromMaze();
}

void Window::onRunExit(int exitCode, QProcess::ExitStatus exitStatus) {

    // Always unpause on exit
    if (m_isPaused) {
        onPauseButtonPressed();
    }

    // Only enabled while mouse is running
    m_pauseButton->setEnabled(false);
    m_resetButton->setEnabled(false);
    m_resetButton->setText("Reset");
    m_wasReset = false;

    // Update the run button
    disconnect(
        m_runButton,
        &QPushButton::clicked,
        this,
        &Window::cancelRun
    );
    connect(
        m_runButton,
        &QPushButton::clicked,
        this,
        &Window::startRun
    );
    m_runButton->setText("Run");

    // Update the status label
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        m_runStatus->setText("COMPLETE");
        m_runStatus->setStyleSheet(COMPLETE_STYLE_SHEET);
    }
    else {
        m_runStatus->setText("FAILED");
        m_runStatus->setStyleSheet(FAILED_STYLE_SHEET);
    }

    // Clean up (stop producing commands)
    delete m_runProcess;
    m_runProcess = nullptr;

    // Stop consuming queued commands
    m_commandQueueTimer->stop();
    m_commandQueue.clear();
}

void Window::removeMouseFromMaze() {

    // No-op if no mouse
    if (m_mouse == nullptr) {
        return;
    }

    // Update some objects
    m_map->setView(m_truth);
    m_map->setMouseGraphic(nullptr);

    // Delete some objects
    ASSERT_FA(m_view == nullptr);
    ASSERT_FA(m_mouseGraphic == nullptr);
    delete m_mouse;
    m_mouse = nullptr;
    delete m_view;
    m_view = nullptr;
    delete m_mouseGraphic;
    m_mouseGraphic = nullptr;

    // Reset communication state
    m_logBuffer.clear();
    m_commandBuffer.clear();

    // Reset movement state
    m_startingLocation = {0, 0};
    m_startingDirection = Direction::NORTH;
    m_movement = Movement::NONE;
    m_movementProgress = 0.0;
    m_movementStepSize = 0.0;

    // Reset other mouse state
    m_tilesWithColor.clear();
    m_tilesWithText.clear();
}

void Window::onPauseButtonPressed() {
    m_isPaused = !m_isPaused;
    if (m_isPaused) {
        m_pauseButton->setText("Resume");
        m_runStatus->setText("PAUSED");
    }
    else {
        m_pauseButton->setText("Pause");
        m_runStatus->setText("RUNNING");
        processQueuedCommands();
    }
}

void Window::onResetButtonPressed() {
    m_resetButton->setEnabled(false);
    m_resetButton->setText("Waiting");
    m_wasReset = true;
}

QStringList Window::processText(QString text, QStringList* buffer) {

    QStringList lines;

    // Separate the text by line
    text.replace("\r", "");  // Windows compatibility
    QStringList parts = text.split("\n");

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

void Window::dispatchCommand(QString command) {

    // For performance reasons, handle no-response commands inline (don't queue
    // them with the commands that elicit a response, just perform the action)
    if (
        command.startsWith("setWall") ||
        command.startsWith("clearWall")
    ) {
        QStringList tokens = command.split(" ", QString::SkipEmptyParts);
        if (tokens.size() != 4) {
            return;
        }
        if (!(tokens.at(0) == "setWall" || tokens.at(0) == "clearWall")) {
            return;
        }
        bool ok = true;
        int x = tokens.at(1).toInt(&ok);
        int y = tokens.at(2).toInt(&ok);
        if (!ok) {
            return;
        }
        if (tokens.at(3).size() != 1) {
            return;
        }
        QChar direction = tokens.at(3).at(0);
        if (!CHAR_TO_DIRECTION().contains(direction)) {
            return;
        }
        if (command.startsWith("setWall")) {
            setWall(x, y, direction);
        }
        else if (command.startsWith("clearWall")) {
            clearWall(x, y, direction);
        }
        else {
            ASSERT_NEVER_RUNS();
        }
    }
    else if (command.startsWith("setColor")) {
        QStringList tokens = command.split(" ", QString::SkipEmptyParts);
        if (tokens.size() != 4) {
            return;
        }
        if (tokens.at(0) != "setColor") {
            return;
        }
        bool ok = true;
        int x = tokens.at(1).toInt(&ok);
        int y = tokens.at(2).toInt(&ok);
        if (!ok) {
            return;
        }
        if (tokens.at(3).size() != 1) {
            return;
        }
        QChar color = tokens.at(3).at(0);
        if (!CHAR_TO_COLOR().contains(color)) {
            return;
        }
        setColor(x, y, color);
    }
    else if (command.startsWith("clearColor")) {
        QStringList tokens = command.split(" ", QString::SkipEmptyParts);
        if (tokens.size() != 3) {
            return;
        }
        if (tokens.at(0) != "clearColor") {
            return;
        }
        bool ok = true;
        int x = tokens.at(1).toInt(&ok);
        int y = tokens.at(2).toInt(&ok);
        if (!ok) {
            return;
        }
        clearColor(x, y);
    }
    else if (command.startsWith("clearAllColor")) {
        QStringList tokens = command.split(" ", QString::SkipEmptyParts);
        if (tokens.size() != 1) {
            return;
        }
        if (tokens.at(0) != "clearAllColor") {
            return;
        }
        clearAllColor();
    }
    else if (command.startsWith("setText")) {
        // Special parsing to allow space characters in the text
        int firstSpace = command.indexOf(" ");
        int secondSpace = command.indexOf(" ", firstSpace + 1);
        int thirdSpace = command.indexOf(" ", secondSpace + 1);
        QString function = command.left(firstSpace);
        if (function != "setText") {
            return;
        }
        QString xString = command.mid(firstSpace + 1, secondSpace - firstSpace);
        QString yString = command.mid(secondSpace + 1, thirdSpace - secondSpace);
        bool ok = true;
        int x = xString.toInt(&ok);
        int y = yString.toInt(&ok);
        if (!ok) {
            return;
        }
        QString text = command.mid(thirdSpace + 1);
        setText(x, y, text);
    }
    else if (command.startsWith("clearText")) {
        QStringList tokens = command.split(" ", QString::SkipEmptyParts);
        if (tokens.size() != 3) {
            return;
        }
        if (tokens.at(0) != "clearText") {
            return;
        }
        bool ok = true;
        int x = tokens.at(1).toInt(&ok);
        int y = tokens.at(2).toInt(&ok);
        if (!ok) {
            return;
        }
        clearText(x, y);
    }
    else if (command.startsWith("clearAllText")) {
        QStringList tokens = command.split(" ", QString::SkipEmptyParts);
        if (tokens.size() != 1) {
            return;
        }
        if (tokens.at(0) != "clearAllText") {
            return;
        }
        clearAllText();
    }
    else {
        // Enqueue the serial command, process it if
        // future processing is not already scheduled
        m_commandQueue.enqueue(command);
        if (!m_commandQueueTimer->isActive()) {
            processQueuedCommands();
        }
    }
}

QString Window::executeCommand(QString command) {
    QStringList tokens = command.split(" ", QString::SkipEmptyParts);
    if (tokens.size() < 1 || tokens.size() > 2) {
        return INVALID;
    }
    QString function = tokens.at(0);
    if (tokens.size() == 2 &&
            !(function == "moveForward" || function == "getStat")) {
        return INVALID;
    }
    if (function == "mazeWidth") {
        return QString::number(mazeWidth());
    }
    else if (function == "mazeHeight") {
        return QString::number(mazeHeight());
    }
    else if (function == "wallFront") {
        return boolToString(wallFront(0));
    }
    else if (function == "wallRight") {
        return boolToString(wallRight());
    }
    else if (function == "wallLeft") {
        return boolToString(wallLeft());
    }
    else if (function == "moveForward") {
        int distance = 1;
        if (tokens.size() == 2) {
            distance = tokens.at(1).toInt();
        }
        bool success = moveForward(distance);
        return success ? "" : CRASH;
    }
    else if (function == "turnRight") {
        turnRight();
        return "";
    }
    else if (function == "turnLeft") {
        turnLeft();
        return "";
    }
    else if (function == "wasReset") {
        return boolToString(wasReset());
    }
    else if (function == "ackReset") {
        ackReset();
        return ACK;
    }
    else if (function == "getStat") {
        if (tokens.size() != 2) {
            return INVALID;
        }
        QString stat = tokens.at(1);
        // Convert stat to a StatsEnum
        StatsEnum statsEnum;
        if (stat == "total-distance") {
            statsEnum = StatsEnum::TOTAL_DISTANCE;
        }
        else if (stat == "total-turns") {
            statsEnum = StatsEnum::TOTAL_TURNS;
        }
        else if (stat == "best-run-distance") {
            statsEnum = StatsEnum::BEST_RUN_DISTANCE;
        }
        else if (stat == "best-run-turns") {
            statsEnum = StatsEnum::BEST_RUN_TURNS;
        }
        else if (stat == "current-run-distance") {
            statsEnum = StatsEnum::CURRENT_RUN_DISTANCE;
        }
        else if (stat == "current-run-turns") {
            statsEnum = StatsEnum::CURRENT_RUN_TURNS;
        }
        else if (stat == "total-effective-distance") {
            statsEnum = StatsEnum::TOTAL_EFFECTIVE_DISTANCE;
        }
        else if (stat == "best-run-effective-distance") {
            statsEnum = StatsEnum::BEST_RUN_EFFECTIVE_DISTANCE;
        }
        else if (stat == "current-run-effective-distance") {
            statsEnum = StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE;
        }
        else if (stat == "score") {
            statsEnum = StatsEnum::SCORE;
        }
        else {
            return INVALID;
        }
        QString statValue = stats->getStat(statsEnum);
        if (statValue == "") {
            // Cannot return an empty string. Return -1 to indicate empty field.
            return "-1";
        }
        return statValue;
    }
    else {
        return INVALID;
    }
}

void Window::processQueuedCommands() {
    while (!m_commandQueue.isEmpty() && !m_isPaused) {
        QString response = "";
        if (isMoving()) {
            updateMouseProgress(m_movementStepSize);
            if (!isMoving()) {
                if (m_doomedToCrash) {
                    response = CRASH;
                }
                else {
                    response = ACK;
                }
            }
        }
        else {
            response = executeCommand(m_commandQueue.head());
        }
        if (!response.isEmpty()) {
            // Drop all invalid commands on the floor
            if (response != INVALID) {
                m_runProcess->write((response + "\n").toStdString().c_str());
            }
            m_commandQueue.dequeue();
        }
        else {
            scheduleMouseProgressUpdate();
            break;
        }
    }
}

double Window::progressRequired(Movement movement) {
    switch (movement) {
        case Movement::MOVE_FORWARD:
            return PROGRESS_REQUIRED_FOR_MOVE;
        case Movement::TURN_RIGHT:
        case Movement::TURN_LEFT:
            return PROGRESS_REQUIRED_FOR_TURN;
        default:
            ASSERT_NEVER_RUNS();
    }
}

void Window::updateMouseProgress(double progress) {

    // Determine the destination of the mouse.
    QPair<int, int> destinationLocation = m_startingLocation;
    Angle destinationRotation =
        DIRECTION_TO_ANGLE().value(m_startingDirection);
    if (m_movement == Movement::MOVE_FORWARD) {
        if (m_startingDirection == Direction::NORTH) {
            destinationLocation.second += 1;
        }
        else if (m_startingDirection == Direction::EAST) {
            destinationLocation.first += 1;
        }
        else if (m_startingDirection == Direction::SOUTH) {
            destinationLocation.second -= 1;
        }
        else if (m_startingDirection == Direction::WEST) {
            destinationLocation.first -= 1;
        }
        else {
            ASSERT_NEVER_RUNS();
        }
    }
    // Explicity add or subtract 90 degrees so that the mouse is guaranteed to
    // only rotate 90 degrees (using DIRECTION_ROTATE can cause the mouse to
    // rotate 270 degrees in the opposite direction in some cases)
    else if (m_movement == Movement::TURN_RIGHT) {
        destinationRotation -= Angle::Degrees(90);
    }
    else if (m_movement == Movement::TURN_LEFT) {
        destinationRotation += Angle::Degrees(90);
    }
    else {
        ASSERT_NEVER_RUNS();
    }

    // Increment the movement progress, calculate fraction complete
    m_movementProgress += progress;
    double required = progressRequired(m_movement);
    double remaining = required - m_movementProgress;
    if (remaining < 0) {
        remaining = 0;
    }
    double fraction = 1.0 - (remaining / required);

    // Calculate the current translation and rotation
    Coordinate startingTranslation =
        getCenterOfTile(m_startingLocation.first, m_startingLocation.second);
    Coordinate destinationTranslation =
        getCenterOfTile(destinationLocation.first, destinationLocation.second);
    Angle startingRotation =
        DIRECTION_TO_ANGLE().value(m_startingDirection);
    Coordinate currentTranslation =
        startingTranslation * (1.0 - fraction) +
        destinationTranslation * fraction;
    Angle currentRotation =
        startingRotation * (1.0 - fraction) +
        destinationRotation * fraction;

    // Teleport the mouse, reset movement state if done
    m_mouse->teleport(currentTranslation, currentRotation);
    if (remaining == 0.0) {
        m_startingLocation = m_mouse->getCurrentDiscretizedTranslation();
        m_startingDirection = m_mouse->getCurrentDiscretizedRotation();
        m_movementProgress = 0.0;
        m_movementStepSize = 0.0;
        if (m_movement == Movement::MOVE_FORWARD) {
            m_movesRemaining -= 1;
        }
        if (m_movesRemaining == 0) {
            m_movement = Movement::NONE;
        }
        // determine if the goal was reached
        if (m_maze->isInCenter(m_startingLocation)) {
            stats->finishRun(); // record a completed start-to-finish run
        }
        else if (m_startingLocation.first == 0 && m_startingLocation.second == 0) {
            stats->endUnfinishedRun();
        }
    }
}

void Window::scheduleMouseProgressUpdate() {
    
    // Calculate progressRemaining, should be nonzero
    double required = progressRequired(m_movement);
    double progressRemaining = required - m_movementProgress;
    ASSERT_LT(0.0, progressRemaining);

    // Calculate progressPerSecond for non-linear slider
    double value = static_cast<double>(m_speedSlider->value());
    double fraction = value / SPEED_SLIDER_MAX;
    double rangeMin = qPow(MIN_PROGRESS_PER_SECOND, .25);
    double rangeMax = qPow(MAX_PROGRESS_PER_SECOND, .25);
    double rangeValue = (1.0 - fraction) * rangeMin + fraction * rangeMax;
    double progressPerSecond = qPow(rangeValue, 4);

    // Determine seconds remaing
    double secondsRemaining = progressRemaining / progressPerSecond;
    if (secondsRemaining > MAX_SLEEP_SECONDS) {
        secondsRemaining = MAX_SLEEP_SECONDS;
        progressRemaining = secondsRemaining * progressPerSecond;
    }

    // Update step size, set the timer
    m_movementStepSize = progressRemaining;
    m_commandQueueTimer->start(secondsRemaining * 1000);
}

bool Window::isMoving() {
    return m_movement != Movement::NONE;
}

void Window::createStat(QString name, enum StatsEnum stat, int labelRow, int labelCol, int valueRow, int valueCol, QGridLayout* layout) {
    QLabel* label = new QLabel(name);
    layout->addWidget(label, labelRow, labelCol);
    QLineEdit* textbox = new QLineEdit();
    textbox->setReadOnly(true);
    stats->bindText(stat, textbox);
    layout->addWidget(textbox, valueRow, valueCol);
}

int Window::mazeWidth() {
    return m_maze->getWidth();
}

int Window::mazeHeight() {
    return m_maze->getHeight();
}

bool Window::wallFront(int distance) {
    QPair<int, int> position = m_mouse->getCurrentDiscretizedTranslation();
    Direction direction = m_mouse->getCurrentDiscretizedRotation();
    switch (direction) {
        case Direction::NORTH:
            position.second += distance;
            break;
        case Direction::SOUTH:
            position.second -= distance;
            break;
        case Direction::EAST:
            position.first += distance;
            break;
        case Direction::WEST:
            position.first -= distance;
            break;
    }
    return isWall({position.first, position.second, direction});
}

bool Window::wallRight() {
    QPair<int, int> position = m_mouse->getCurrentDiscretizedTranslation();
    Direction direction =
        DIRECTION_ROTATE_RIGHT().value(m_mouse->getCurrentDiscretizedRotation());
    return isWall({position.first, position.second, direction});
}

bool Window::wallLeft() {
    QPair<int, int> position = m_mouse->getCurrentDiscretizedTranslation();
    Direction direction =
        DIRECTION_ROTATE_LEFT().value(m_mouse->getCurrentDiscretizedRotation());
    return isWall({position.first, position.second, direction});
}

bool Window::moveForward(int distance) {
    // Non-positive distances aren't allowed
    if (distance < 1) {
        return false;
    }
    // Special case for a wall directly in front of the mouse, else
    // the wall won't be detected until after the mouse starts moving
    if (wallFront(0)) {
        return false;
    }
    // Compute the number of allowable moves
    int moves = 1;
    while (moves < distance) {
        if (wallFront(moves)) {
            break;
        }
        moves += 1;
    }
    m_movement = Movement::MOVE_FORWARD;
    m_doomedToCrash = (moves != distance);
    m_movesRemaining = moves;
    if (m_startingLocation.first == 0 && m_startingLocation.second == 0) {
        stats->startRun();
    }
    // increase the stats by the distance that will be travelled
    stats->addDistance(moves);
    return true;
}

void Window::turnRight() {
    m_movement = Movement::TURN_RIGHT;
    m_doomedToCrash = false;
    m_movesRemaining = 0;
    stats->addTurn();
}

void Window::turnLeft() {
    m_movement = Movement::TURN_LEFT;
    m_doomedToCrash = false;
    m_movesRemaining = 0;
    stats->addTurn();
}

void Window::setWall(int x, int y, QChar direction) {
    if (!isWithinMaze(x, y)) {
        return;
    }
    if (!CHAR_TO_DIRECTION().contains(direction)) {
        return;
    }
    Direction d = CHAR_TO_DIRECTION().value(direction);
    m_view->getMazeGraphic()->setWall(x, y, d);
    Wall opposingWall = getOpposingWall({x, y, d});
    if (isWithinMaze(opposingWall.x, opposingWall.y)) {
        m_view->getMazeGraphic()->setWall(
            opposingWall.x,
            opposingWall.y,
            opposingWall.d
        ); 
    }
}

void Window::clearWall(int x, int y, QChar direction) {
    if (!isWithinMaze(x, y)) {
        return;
    }
    if (!CHAR_TO_DIRECTION().contains(direction)) {
        return;
    }
    Direction d = CHAR_TO_DIRECTION().value(direction);
    m_view->getMazeGraphic()->clearWall(x, y, d);
    Wall opposingWall = getOpposingWall({x, y, d});
    if (isWithinMaze(opposingWall.x, opposingWall.y)) {
        m_view->getMazeGraphic()->clearWall(
            opposingWall.x,
            opposingWall.y,
            opposingWall.d
        ); 
    }
}

void Window::setColor(int x, int y, QChar color) {
    if (!isWithinMaze(x, y)) {
        return;
    }
    if (!CHAR_TO_COLOR().contains(color)) {
        return;
    }
    m_view->getMazeGraphic()->setColor(x, y, CHAR_TO_COLOR().value(color));
    m_tilesWithColor.insert({x, y});
}

void Window::clearColor(int x, int y) {
    if (!isWithinMaze(x, y)) {
        return;
    }
    m_view->getMazeGraphic()->clearColor(x, y);
    m_tilesWithColor -= {x, y};
}

void Window::clearAllColor() {
    for (QPair<int, int> position : m_tilesWithColor) {
        m_view->getMazeGraphic()->clearColor(position.first, position.second);
    }
    m_tilesWithColor.clear();
}

void Window::setText(int x, int y, QString text) {
    if (!isWithinMaze(x, y)) {
        return;
    }
    static QRegExp regex = QRegExp(
        QString("[^") + FontImage::characters() + QString("]")
    );
    text.replace(regex, "?");
    m_view->getMazeGraphic()->setText(x, y, text);
    m_tilesWithText.insert({x, y});
}

void Window::clearText(int x, int y) {
    if (!isWithinMaze(x, y)) {
        return;
    }
    m_view->getMazeGraphic()->clearText(x, y);
    m_tilesWithText -= {x, y};
}

void Window::clearAllText() {
    for (QPair<int, int> position : m_tilesWithText) {
        m_view->getMazeGraphic()->clearText(position.first, position.second);
    }
    m_tilesWithText.clear();
}

bool Window::wasReset() {
    return m_wasReset;
}

void Window::ackReset() {
    m_mouse->reset();
    m_startingLocation = {0, 0};
    m_startingDirection = Direction::NORTH;
    m_movement = Movement::NONE;
    m_movementProgress = 0.0;
    m_movementStepSize = 0.0;
    m_resetButton->setEnabled(true);
    m_resetButton->setText("Reset");
    m_wasReset = false;
    stats->penalizeForReset();
    stats->endUnfinishedRun();
}

QString Window::boolToString(bool value) const {
    return value ? "true" : "false";
}

bool Window::isWall(Wall wall) const {
    return m_maze->getTile(wall.x, wall.y)->isWall(wall.d);
}

bool Window::isWithinMaze(int x, int y) const {
    return (
        0 <= x && x < m_maze->getWidth() &&
        0 <= y && y < m_maze->getHeight()
    );
}

Wall Window::getOpposingWall(Wall wall) const {
    switch (wall.d) {
        case Direction::NORTH:
            return {wall.x, wall.y + 1, Direction::SOUTH};
        case Direction::EAST:
            return {wall.x + 1, wall.y, Direction::WEST};
        case Direction::SOUTH:
            return {wall.x, wall.y - 1, Direction::NORTH};
        case Direction::WEST:
            return {wall.x - 1, wall.y, Direction::EAST};
    }
}

Coordinate Window::getCenterOfTile(int x, int y) const {
    ASSERT_TR(isWithinMaze(x, y));
    Coordinate centerOfTile = Coordinate::Cartesian(
        Dimensions::tileLength() * (static_cast<double>(x) + 0.5),
        Dimensions::tileLength() * (static_cast<double>(y) + 0.5)
    );
    return centerOfTile;
}

} 
