#include "Driver.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>

// TODO: MACK - replace this with QThread
#include <thread>

#include "Assert.h"
#include "Directory.h"
#include "Logging.h"
#include "MainWindow.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"
#include "units/Milliseconds.h"

namespace mms {

// Definition of the static variables for linking
Model* Driver::m_model;
View* Driver::m_view;
Controller* Driver::m_controller;

int Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    ASSERT_RUNS_JUST_ONCE();

    // Initialize Qt
    QApplication app(argc, argv);

    // Initialize the Time object
    Time::init();

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
    m_model = new Model();
    m_view = new View(m_model, argc, argv);

    // Initialize the controller, which starts the algorithm
    // (and returns once the static options have been set)
    m_controller = new Controller(m_model, m_view);

    // Initialize mouse algorithm values in the model and view
    m_model->getWorld()->setOptions(
        m_controller->getStaticOptions()
    );
    m_view->setController(m_controller);

    // Initialize the tile text, now that the options have been set
    m_view->initTileGraphicText();

    // Lastly, we need to populate the graphics buffers with maze information,
    // but only after we've initialized the tile graphic text
    m_view->getMazeGraphic()->draw();

    //
    /////////////////////////////////////////

    // TODO: MACK - this could be on the same thread as the graphics loop
    // Start the physics loop
    std::thread physicsThread([]() {
        m_model->getWorld()->simulate();
    });

    // TODO: MACK -- create the main window
    MainWindow w(m_view);
    w.show();
    // TODO: MACK

    // Start the event loop
    return app.exec();
}

} // namespace mms
