#include "Driver.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QProcess>

// TODO: MACK - replace this with QThread
#include <thread>

#include "Assert.h"
#include "ControllerManager.h"
#include "Directory.h"
#include "FontImage.h"
#include "Logging.h"
#include "MainWindow.h"
#include "Model.h"
#include "Param.h"
#include "Screen.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"
#include "Map.h"
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

    // Initialize the model and lens
    Model* model = new Model();
    Lens* lens = new Lens(model);

    // Initialize the controllerManager, which starts the algorithm
    ControllerManager* controllerManager = new ControllerManager(model, lens);
    Controller* controller =
        controllerManager->spawnMouseAlgo(P()->mouseAlgorithm());

    // TODO: MACK - this could be on the same thread as the graphics loop
    // Start the physics loop
    std::thread physicsThread([=]() {
        model->getWorld()->simulate();
    });

    // TODO: MACK -- create the main window
    MainWindow w(model, lens, controller);
    w.show();
    // TODO: MACK

    // Start the event loop
    return app.exec();
}

} // namespace mms
