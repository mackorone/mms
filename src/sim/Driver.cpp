#include "Driver.h"

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

// TODO: MACK - replace this with QThread
#include <thread>

#include "Assert.h"
#include "Directory.h"
#include "Logging.h"
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

void Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    SIM_ASSERT_RUNS_JUST_ONCE();

    // Initialize the Qt core application
    QCoreApplication app(argc, argv);

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

    // Generate the glut functions in a static context
    GlutFunctions functions = {
        []() {
            m_view->refresh();
        },
        [](int width, int height) {
            m_view->updateWindowSize(width, height);
        },
        [](unsigned char key, int x, int y) {
            m_view->keyPress(key, x, y);
        },
        [](int key, int x, int y) {
            m_view->specialKeyPress(key, x, y);
        },
        [](int key, int x, int y) {
            m_view->specialKeyRelease(key, x, y);
        }
    };

    // Initialize the model and view
    m_model = new Model();
    m_view = new View(m_model, argc, argv, functions);

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

    // TODO: MACK - this could be on the same thread as the graphics loop
    // Start the physics loop
    std::thread physicsThread([]() {
        m_model->getWorld()->simulate();
    });

    // Start the graphics loop on a separate thread, allows the graphics to
    // refresh while mouse commands (e.g., moveForward()) can be handled
    // synchronously in the main thread.
    std::thread graphicsThread([]() {
        glutMainLoop();
    });

    // We need to process mouse algo events on the main thread.
    while (true) {
        SimUtilities::sleep(Milliseconds(1));
        QCoreApplication::processEvents();
    }
}

} // namespace mms
