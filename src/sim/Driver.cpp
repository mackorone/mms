#include "Driver.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QObject>
#include <QProcess>
#include <QThread>

#include "ControllerManager.h"
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

    // TODO: MACK - clean this up

    // Create the model and start the physics loop
    Model model;
    QThread modelThread;
    QObject::connect(&modelThread, &QThread::started, &model, &Model::simulate);
    model.moveToThread(&modelThread);
    modelThread.start();

    Maze* maze = new Maze();
    Mouse* mouse = new Mouse(maze);

    model.setMaze(maze);
    model.addMouse("", mouse); // TODO: MACK - name

    // Initialize the model and lens
    Lens* lens = new Lens(maze, mouse);

    // Initialize the controllerManager, which starts the algorithm
    ControllerManager* controllerManager =
        new ControllerManager(&model, maze, mouse, lens);
    Controller* controller =
        controllerManager->spawnMouseAlgo(P()->mouseAlgorithm());

    // TODO: MACK -- create the main window
    MainWindow w(&model, maze, mouse, lens, controller);
    w.show();
    // TODO: MACK

    // Start the event loop
    return app.exec();
}

} // namespace mms
