#include "Driver.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QProcess>

// TODO: MACK - replace this with QThread
#include <thread>

#include "Assert.h"
#include "ControllerManager.h"
#include "Directory.h"
#include "Logging.h"
#include "MainWindow.h"
#include "Model.h"
#include "Param.h"
#include "Screen.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"
#include "View.h"
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

    // Remove any excessive archived runs
    SimUtilities::removeExcessArchivedRuns();

    /////////////////////////////////////////
    // TODO: MACK - clean this up a little bit

    // Initialize the model and view
    Model* model = new Model();
    View* view = new View(model);

    // Initialize the controllerManager, which starts the algorithm
    ControllerManager* controllerManager = new ControllerManager(model, view);
    controllerManager->spawnMouseAlgo(P()->mouseAlgorithm());

    // Initialize mouse algorithm values in the model and view
    // TODO: MACK
    /*
    model->getWorld()->setOptions(
        controllerManager->getStaticOptions()
    );
    */
    view->setControllerManager(controllerManager);

    // Initialize the tile text, now that the options have been set
    view->initTileGraphicText();

    // Lastly, we need to populate the graphics buffers with maze information,
    // but only after we've initialized the tile graphic text
    view->getMazeGraphic()->draw();

    //
    /////////////////////////////////////////

    // TODO: MACK - this could be on the same thread as the graphics loop
    // Start the physics loop
    std::thread physicsThread([=]() {
        model->getWorld()->simulate();
    });

    // TODO: MACK -- create the main window
    MainWindow w(view);
    w.show();
    // TODO: MACK

    // Start the event loop
    return app.exec();
}

} // namespace mms
