#include "Driver.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QObject>
#include <QProcess>
#include <QThread>

#include "Directory.h"
#include "FontImage.h"
#include "Logging.h"
#include "MainWindow.h"
#include "Map.h"
#include "Maze.h"
#include "Mouse.h"
#include "Param.h"
#include "Screen.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"
#include "Model.h"
#include "units/Milliseconds.h"

namespace mms {

int Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    ASSERT_RUNS_JUST_ONCE();

    // Initialize Qt
    QApplication app(argc, argv);

    // Initialize the Time object
    Time::init();

    // Initialize the Screen object
    Screen::init();

    // Initialize the Directory object
    Directory::init(app.applicationFilePath());

    // Then, determine the runId (just datetime for now)
    QString runId = SimUtilities::timestampToDatetimeString(
        Time::get()->startTimestamp()
    );

    // Then, initiliaze logging (before initializing Param or State)
    Logging::init(runId);

    // Initialize the State object in order to:
    // 1) Set the runId
    // 2) Avoid a race condition (between threads)
    // 3) Initialize the Param object
    S()->setRunId(runId);

    // Initialize the FontImage object
    FontImage::init(
        Directory::get()->getResImgsDirectory() +
        P()->tileTextFontImage());

    // Remove any excessive archived runs
    SimUtilities::removeExcessArchivedRuns();

    // Initialize the model, start the physics loop
    Model::init();
    QThread modelThread;
    QObject::connect(
        &modelThread, &QThread::started,
        Model::get(), &Model::simulate);
    Model::get()->moveToThread(&modelThread);
    modelThread.start();

    // Create the maze, add it to the model
    Maze maze;
    Model::get()->setMaze(&maze);

    // Create the main window
    MainWindow window(&maze);
    window.show();
    // Ensure key-press events get sent to the window
    app.installEventFilter(&window);

    // Start the event loop
    return app.exec();
}

} // namespace mms
