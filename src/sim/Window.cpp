#include "Window.h"

#include <QAction>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLinkedList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QShortcut>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>

#include "Assert.h"
#include "Color.h"
#include "ColorManager.h"
#include "ConfigDialog.h"
#include "Dimensions.h"
#include "FontImage.h"
#include "ProcessUtilities.h"
#include "Resources.h"
#include "SettingsMouseAlgos.h"
#include "SettingsMisc.h"
#include "SimUtilities.h"

namespace mms {

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    m_map(new Map()),
    m_maze(nullptr),
    m_truth(nullptr),
    m_mouse(nullptr),
    m_mouseGraphic(nullptr),
    m_view(nullptr),
    m_commandQueueTimer(new QTimer()),
    m_movement(Movement::NONE),
    m_progress(0.0),

    // TODO: MACK
    m_step(0.0),

    // MouseAlgosTab
    m_mouseAlgoComboBox(new QComboBox()),
    m_mouseAlgoEditButton(new QToolButton()),
    m_mouseAlgoImportButton(new QToolButton()),
    m_buildProcess(nullptr),
    m_buildButton(new QPushButton("Build")),
    m_buildStatus(new QLabel()),
    m_buildOutput(new QPlainTextEdit()),
    m_runProcess(nullptr),
    m_runButton(new QPushButton("Run")),
    m_runStatus(new QLabel()),
    m_runOutput(new QPlainTextEdit()),
    m_resetButton(new QPushButton("Reset")),
    m_pauseButton(new QPushButton("Pause")),
    m_speedSlider(new QSlider(Qt::Horizontal)),
    m_isPaused(false),
    m_wasReset(false) {

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);
    connect(shortcut, &QShortcut::activated, this, [=](){close();});
    QShortcut* sc = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this);
    connect(sc, &QShortcut::activated, this, [=](){close();});

    // Add the splitter to the window
    QSplitter* splitter = new QSplitter();
    splitter->setHandleWidth(6);
    setCentralWidget(splitter);

    // Add a container for the maze stats, map and options
    QWidget* mapHolder = new QWidget();
    QVBoxLayout* mapHolderLayout = new QVBoxLayout();
    mapHolder->setLayout(mapHolderLayout);
    splitter->addWidget(mapHolder);

    // Add the map (and set some layout props)
    mapHolderLayout->addWidget(m_map);
    mapHolderLayout->setContentsMargins(0, 0, 0, 0);
    mapHolderLayout->setSpacing(0);
    m_map->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //  ----- Create the mouse algos tab

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
    QWidget* panel = new QWidget();
    panel->setLayout(layout);
    QGridLayout* topLayout = new QGridLayout();
    layout->addLayout(topLayout);

    // Config 
    QGroupBox* configGroupBox = new QGroupBox("Config");
    topLayout->addWidget(configGroupBox, 0, 1, 1, 1);
    QGridLayout* configLayout = new QGridLayout();
    configGroupBox->setLayout(configLayout);

    // Maze
    QLabel* mazeLabel = new QLabel("Maze");
    QLabel* mouseLabel = new QLabel("Mouse");
    mazeLabel->setAlignment(Qt::AlignCenter);
    mouseLabel->setAlignment(Qt::AlignCenter);
    mazeLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    mouseLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));


    configLayout->addWidget(mazeLabel, 0, 0, 1, 1);
    QComboBox* mazeBox = new QComboBox();
    mazeBox->addItem("A");
    mazeBox->addItem("B");
    configLayout->addWidget(mazeBox, 0, 1, 1, 2);


    // TODO: Add the default mazes

    /*
    for (const QString& path : Resources::getMazes()) {
        ASSERT_TR(path.startsWith(Resources::getMazesPath()));
        QString name = path.right(
            path.size() -
            Resources::getMazesPath().size()
        );
        connect(action, &QAction::triggered, this, [=](){
            loadMazeFile(path);
        });
        chooseMazeMenu->addAction(action);
    }
    */



    QToolButton* button = new QToolButton();
    button->setIcon(QIcon(":/resources/icons/open.png"));

    m_mouseAlgoEditButton->setIcon(QIcon(":/resources/icons/edit.png"));
    m_mouseAlgoImportButton->setIcon(QIcon(":/resources/icons/plus.png"));

    configLayout->addWidget(button, 0, 3, 1, 1);
    connect(
        button, &QToolButton::clicked,
        this, [=](){
            QString path = QFileDialog::getOpenFileName(
                this,
                tr("Load Maze")
                //"",
                //tr("Images (*.png *.xpm *.jpg)")
            );
            if (!path.isNull()) {
                loadMazeFile(path);
            }
        }
    );

    // Mouse
    configLayout->addWidget(mouseLabel, 1, 0, 1, 1);
    configLayout->addWidget(m_mouseAlgoComboBox, 1, 1, 1, 2);
    configLayout->addWidget(m_mouseAlgoEditButton, 1, 3, 1, 1);
    configLayout->addWidget(m_mouseAlgoImportButton, 1, 4, 1, 1);
    connect(
        m_mouseAlgoComboBox, &QComboBox::currentTextChanged,
        this, [=](QString name){
            SettingsMisc::setRecentMouseAlgo(name);
        }
    );

    // Actions groupbox
    QGroupBox* controlsGroupBox = new QGroupBox("Controls");
    topLayout->addWidget(controlsGroupBox, 0, 0, 1, 1);
    QGridLayout* controlsLayout = new QGridLayout();
    controlsGroupBox->setLayout(controlsLayout);

    controlsLayout->addWidget(m_buildButton, 0, 0);
    controlsLayout->addWidget(m_buildStatus, 0, 1);
    controlsLayout->addWidget(m_runButton, 1, 0);
    controlsLayout->addWidget(m_runStatus, 1, 1);
    for (QLabel* label : {m_buildStatus, m_runStatus}) {
        label->setAlignment(Qt::AlignCenter);   
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        label->setMinimumWidth(90);
    }

    // Runtime controls group box
    controlsLayout->addWidget(m_pauseButton, 0, 2, 1, 1);
    controlsLayout->addWidget(m_resetButton, 0, 3, 1, 1);
    double maxSpeed = 3000.0; // progress points per second
    m_speedSlider->setRange(1, 7);
    m_speedSlider->setTickInterval(1);
    controlsLayout->addWidget(m_speedSlider, 1, 2, 1, 2);

    // Set up the pause button initial state
    m_pauseButton->setEnabled(false);

    connect(
        m_resetButton, &QPushButton::pressed,
        this, &Window::resetButtonPressed);

    mouseAlgoResume();

    // Add the build and run output
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    layout->addLayout(bottomLayout);
    m_mouseAlgoOutputTabWidget = new QTabWidget();
    bottomLayout->addWidget(m_mouseAlgoOutputTabWidget);
    m_mouseAlgoOutputTabWidget->addTab(m_buildOutput, "Build Output");
    m_mouseAlgoOutputTabWidget->addTab(m_runOutput, "Run Output");
    m_mouseAlgoOutputTabWidget->addTab(m_runOutput, "Run Output");

    // Set the default values for some widgets
    for (QPlainTextEdit* output : {
        m_buildOutput,
        m_runOutput,
    }) {
        output->setReadOnly(true);
        output->setLineWrapMode(QPlainTextEdit::NoWrap);
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSize(10);
        output->document()->setDefaultFont(font);
    }

    // Add the mouse algos
    mouseAlgoRefresh(SettingsMisc::getRecentMouseAlgo());

    //  ----- Create the mouse algos tab

    splitter->addWidget(panel);

    // Minor layout stuff
    panel->layout()->setContentsMargins(6, 6, 6, 6);

    // Load a maze
    loadMazeFile(Resources::getMazes().at(2));

    // TODO: MACK - remember splitter sizes
    splitter->setSizes({
        SettingsMisc::getRecentWindowHeight(),
        SettingsMisc::getRecentWindowWidth() - SettingsMisc::getRecentWindowHeight()
    });

    // Resize the window
    resize(
        SettingsMisc::getRecentWindowWidth(),
        SettingsMisc::getRecentWindowHeight()
    );

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
            m_map->update();
            then = now;
        }
    );
    mapTimer->start(secondsPerFrame * 1000);

    m_commandQueueTimer->setSingleShot(true);
    connect(
        m_commandQueueTimer, &QTimer::timeout,
        this, &Window::processQueuedCommands
    );
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

void Window::loadMazeFile(QString path) {

    // Create maze object
    Maze* maze = Maze::fromFile(path);
    if (maze == nullptr) {
        QMessageBox::warning(
            this,
            "Invalid Maze File",
            "The following is not a valid maze file:\n\n" + path + "\n\n "
            "The maze must be nonempty, rectangular, enclosed, and consistent."
        );
        return;
    }

    // Stop running maze/mouse algos
    cancelAllProcesses();

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

void Window::processCommand(QString command) {

	// TODO: MACK - print things that don't look like a command
    /*
    HERE'S THE API:
    setColor
    setText
    setWall
    along with clear and clearAll
    */

    // Process no-response commands inline
    if (
        command.startsWith("setColor") ||
        command.startsWith("clearColor") ||
        command.startsWith("clearAllColor") ||
        command.startsWith("setText") ||
        command.startsWith("clearText") ||
        command.startsWith("clearAllText") ||
        command.startsWith("setWall") ||
        command.startsWith("clearWall")
    ) {
        dispatch(command);
        return;
    }

    // Enqueue the serial command, process it if
    // future processing is not already scheduled
    m_commandQueue.enqueue(command);
    if (!m_commandQueueTimer->isActive()) {
        processQueuedCommands();
    }
}

void Window::processQueuedCommands() {
    // TODO: MACK - check for paused
    while (!m_commandQueue.isEmpty() && !m_isPaused) {
        if (isMoving()) {
            // Move a little bit
            // TODO: MACK _ we should actually store the speed value used
            //  double seconds = SimUtilities::getHighResTimestamp() - m_lastTimestamp;
            //  double meters = m_metersPerSecond * seconds;
            //  int ticks = static_cast<int>(meters * 100);
            moveALittle(m_step);
            if (!isMoving()) {
                QString response = "ACK";
                // qDebug() << "ASYNC RESPONSE:" << response;
                m_runProcess->write((response + "\n").toStdString().c_str());
                m_commandQueue.dequeue();
            }
            else {
                scheduleAnotherCall();
                break;
            }
        }
        else {
            QString response = dispatch(m_commandQueue.head());
            if (!response.isEmpty()) {
                m_runProcess->write((response + "\n").toStdString().c_str());
                m_commandQueue.dequeue();
            } else {
                scheduleAnotherCall();
                break;
            }
            continue;
        }
    }
}

void Window::scheduleAnotherCall() {
    double MAX_SECONDS = 0.008;
    double pRemaining = progressRemaining();
    int speed = m_speedSlider->value() * 50;
    double secondsRemaining = pRemaining / speed;
    if (secondsRemaining > MAX_SECONDS) {
        secondsRemaining = MAX_SECONDS;
        pRemaining = secondsRemaining * speed;
    }
    m_step = pRemaining;
    m_commandQueueTimer->start(secondsRemaining * 1000);
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

    // TODO: MACK - delete the font files too

    // Remove the mouse from the maze
    removeMouseFromMaze();

    // Generate the mouse
    m_mouse = new Mouse();
    m_view = new MazeView(m_maze);
    m_mouseGraphic = new MouseGraphic(m_mouse);

    // New interface and process
    QProcess* process = new QProcess();

    // Clear the output, and jump to it
    m_runOutput->clear();
    m_mouseAlgoOutputTabWidget->setCurrentWidget(m_runOutput);

    // Print stdout
    connect(process, &QProcess::readyReadStandardOutput, this, [=](){
        QString output = process->readAllStandardOutput();
        if (output.endsWith("\n")) {
            output.truncate(output.size() - 1);
        }
        m_runOutput->appendPlainText(output);
    });

    // Process commands from stderr
    connect(process, &QProcess::readyReadStandardError, this, [=](){
        QString text = process->readAllStandardError();
        QStringList commands = processText(text);
        for (QString command : commands) {
            processCommand(command);
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

    // Start the run process
    if (ProcessUtilities::start(command, dirPath, process)) {

        m_runProcess = process;
        m_map->setView(m_view);
        m_map->setMouseGraphic(m_mouseGraphic);

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
        m_map->setView(m_view);
        m_pauseButton->setEnabled(true);
    } 
    else {
        // Clean up the failed process
        m_runOutput->appendPlainText(process->errorString());
        m_runStatus->setText("ERROR");
        m_runStatus->setStyleSheet(
            "QLabel { background: rgb(230, 150, 230); }"
        );
        removeMouseFromMaze();
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
    m_commandBuffer.clear();
    // TODO: MACK - make sure this works as expected
    m_commandQueueTimer->stop();
    m_commandQueue.clear();
}

void Window::removeMouseFromMaze() {

    // Put the maze in a mouseless state

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

    // Reset movement related state
    m_movement = Movement::NONE;
    m_progress = 0.0;
    m_wasReset = false;

    // TODO: MACK - most of these shouldn't be necessary
    // Clean up the UI
    mouseAlgoResume();
}

void Window::cancelAllProcesses() {
    cancelBuild();
    cancelRun();
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

void Window::mouseAlgoPause() {
    m_isPaused = true;
    m_pauseButton->setText("Resume");
    disconnect(
        m_pauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoPause
    );
    connect(
        m_pauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoResume
    );
    connect(
        m_pauseButton, &QPushButton::clicked,
        this, &Window::processQueuedCommands
    );
}

void Window::mouseAlgoResume() {
    m_isPaused = false;
    m_pauseButton->setText("Pause");
    disconnect(
        m_pauseButton, &QPushButton::clicked,
        this, &Window::mouseAlgoResume
    );
    connect(
        m_pauseButton, &QPushButton::clicked,
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

QStringList Window::processText(const QString& text) {

    // TODO: upforgrabs
    // Determine whether or not this function is perf sensitive. If so,
    // refactor this so that we're not copying QStrings between lists.

    // Separate the text by line
    QStringList parts = text.split("\n");

    // We'll return list of complete lines
    QStringList lines;

    // If the text has at least one newline character, we definitely have a
    // complete line; combine it with the contents of the buffer and append
    // it to the list of lines to be returned
    if (1 < parts.size()) {
        lines.append(m_commandBuffer.join("") + parts.at(0));
        m_commandBuffer.clear();
    }

    // All newline-separated parts in the text are lines
    for (int i = 1; i < parts.size() - 1; i += 1) {
        lines.append(parts.at(i));
    }

    // Store the last part of the text (empty string if the text ended
    // with newline) in the buffer, to be combined with future input
    m_commandBuffer.append(parts.at(parts.size() - 1));

    return lines;
}

QString Window::dispatch(const QString& command) {

    // TODO: upforgrabs
    // These functions should have sanity checks, e.g., correct
    // types, not finalizing static options more than once, etc.


    // TODO: MACK - set/unset wall? set and clear?

    static const QString ACK_STRING = "ACK";
    static const QString NO_ACK_STRING = "";
    static const QString ERROR_STRING = "!";

    QStringList tokens = command.split(" ", QString::SkipEmptyParts);
    QString function = tokens.at(0);

    // TODO: MACK - if it returns a response, it should be serial

    if (function == "getWidth") {
        return QString::number(getWidth());
    }
    else if (function == "getHeight") {
        return QString::number(getHeight());
    }
    else if (function == "isWallFront") {
        return SimUtilities::boolToStr(isWallFront());
    }
    else if (function == "isWallRight") {
        return SimUtilities::boolToStr(isWallRight());
    }
    else if (function == "isWallLeft") {
        return SimUtilities::boolToStr(isWallLeft());
    }
    else if (function == "moveForward") {
        bool success = moveForward();
        return success ? NO_ACK_STRING : ERROR_STRING;
    }
    else if (function == "turnRight") {
        turnRight();
        return NO_ACK_STRING;
    }
    else if (function == "turnLeft") {
        turnLeft();
        return NO_ACK_STRING;
    }
    else if (function == "setColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QChar color = SimUtilities::strToChar(tokens.at(3));
        setColor(x, y, color);
        return NO_ACK_STRING;
    }
    else if (function == "clearColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        clearColor(x, y);
        return NO_ACK_STRING;
    }
    else if (function == "clearAllColor") {
        clearAllColor();
        return NO_ACK_STRING;
    }
    else if (function == "setText") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        // TODO: MACK - don't allow empty string
        QString text = "";
        if (3 < tokens.size()) {
            text = tokens.at(3);
        }
        setText(x, y, text);
        return NO_ACK_STRING;
    }
    else if (function == "clearText") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        clearText(x, y);
        return NO_ACK_STRING;
    }
    else if (function == "clearAllText") {
        clearAllText();
        return NO_ACK_STRING;
    }
    else if (function == "setWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QChar direction = SimUtilities::strToChar(tokens.at(3));
        bool wallExists = SimUtilities::strToBool(tokens.at(4));
        if (wallExists) {
            setWall(x, y, direction);
        }
        return NO_ACK_STRING;
    }
    else if (function == "clearWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QChar direction = SimUtilities::strToChar(tokens.at(3));
        clearWall(x, y, direction);
        return NO_ACK_STRING;
    }
    else if (function == "wasReset") {
        return SimUtilities::boolToStr(wasReset());
    }
    else if (function == "ackReset") {
        ackReset();
        return ACK_STRING;
    }

    // TODO: MACK - print something here
    return ERROR_STRING;
}

void Window::updateStartingLocationAndDirection() {
    m_startingLocation = m_mouse->getCurrentDiscretizedTranslation();
    m_startingDirection = m_mouse->getCurrentDiscretizedRotation();
}

double Window::progressRequired(Movement movement) {
    switch (movement) {
        case Movement::MOVE_FORWARD:
            return 100.0;
        case Movement::TURN_RIGHT:
        case Movement::TURN_LEFT:
            return 33.33;
        default:
            ASSERT_NEVER_RUNS();
    }
}

bool Window::isMoving() {
    return m_movement != Movement::NONE;
}

double Window::progressRemaining() {
    return progressRequired(m_movement) - m_progress;
}

void Window::moveALittle(double progress) {

    QPair<int, int> destinationLocation = m_startingLocation;
    Angle startingRotation = DIRECTION_TO_ANGLE().value(m_startingDirection);
    Angle destinationRotation = startingRotation;

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
    else if (m_movement == Movement::TURN_RIGHT) {
        destinationRotation -= Angle::Degrees(90);
    }
    else if (m_movement == Movement::TURN_LEFT) {
        destinationRotation += Angle::Degrees(90);
    }
    else {
        ASSERT_NEVER_RUNS();
    }

    m_progress += progress;

    double required = progressRequired(m_movement);
    double remaining = progressRemaining();

    if (remaining < 0.0) {
        remaining = 0.0;
    }

    double two = 1.0 - (remaining / required);
    double one = 1.0 - two;


    // TODO: MACK
    Coordinate start =
        getCenterOfTile(m_startingLocation.first, m_startingLocation.second);
    Coordinate end =
        getCenterOfTile(destinationLocation.first, destinationLocation.second);

    m_mouse->teleport(
        start * one + end * two,
        startingRotation * one + destinationRotation * two
    );

    if (remaining == 0.0) {
        m_movement = Movement::NONE;
        m_progress = 0.0;
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

void Window::resetButtonPressed() {
    // TODO: MACK - need to queue this command
    if (m_runProcess == nullptr) {
        return;
    }
    m_wasReset = true;
    m_commandQueue.clear();
    m_movement = Movement::NONE;
    m_progress = 0.0;
    m_runProcess->write("ACK\n");
    m_mouse->reset();
}

int Window::getWidth() {
    return m_maze->getWidth();
}

int Window::getHeight() {
    return m_maze->getHeight();
}

bool Window::isWallFront() {
    QPair<int, int> position = m_mouse->getCurrentDiscretizedTranslation();
    Direction direction = m_mouse->getCurrentDiscretizedRotation();
    return isWall({position.first, position.second, direction});
}

bool Window::isWallRight() {
    QPair<int, int> position = m_mouse->getCurrentDiscretizedTranslation();
    Direction direction =
        DIRECTION_ROTATE_RIGHT().value(m_mouse->getCurrentDiscretizedRotation());
    return isWall({position.first, position.second, direction});
}

bool Window::isWallLeft() {
    QPair<int, int> position = m_mouse->getCurrentDiscretizedTranslation();
    Direction direction =
        DIRECTION_ROTATE_LEFT().value(m_mouse->getCurrentDiscretizedRotation());
    return isWall({position.first, position.second, direction});
}

bool Window::moveForward() {
    if (isWallFront()) {
        return false;
    }
    updateStartingLocationAndDirection();
    m_movement = Movement::MOVE_FORWARD;
    return true;
}

void Window::turnRight() {
    updateStartingLocationAndDirection();
    m_movement = Movement::TURN_RIGHT;
}

void Window::turnLeft() {
    updateStartingLocationAndDirection();
    m_movement = Movement::TURN_LEFT;
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

void Window::setText(int x, int y, const QString& text) {
    if (!isWithinMaze(x, y)) {
        return;
    }

    // Ensure that all characters are valid
    QString filtered;
    for (int i = 0; i < text.size(); i += 1) {
        QChar c = text.at(i);
        if (!FontImage::get()->positions().contains(c)) {
            qWarning().noquote().nospace()
                << "Unable to set the tile text for unprintable character \""
                << (c == '\n' ? "\\n" :
                   (c == '\t' ? "\\t" :
                   (c == '\r' ? "\\r" : QString(c))))
                << "\". Using the character \"?\" instead.";
            c = '?';
        }
        filtered += c;
    }

    m_view->getMazeGraphic()->setText(x, y, filtered);
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
    m_wasReset = false;
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

} 
