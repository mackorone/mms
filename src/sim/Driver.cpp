#include "Driver.h"

#include <thread>

#include "Assert.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"

namespace sim {

// Definition of the static variables for linking
Model* Driver::m_model;
View* Driver::m_view;
Controller* Driver::m_controller;

void Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    SIM_ASSERT_RUNS_JUST_ONCE();

    // Before anything else, create the Time object
    T();

    // Then, determine the runId (just datetime for now)
    std::string runId = SimUtilities::timestampToDatetimeString(
        T()->startTimestamp()
    );

    // Then, initiliaze logging (before calling P() or S())
    Logging::initialize(runId);

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

    // Initialize the model, view, and controller
    m_model = new Model();
    m_view = new View(m_model, argc, argv, functions);
    m_controller = new Controller(m_model, m_view);

    // Initialize mouse algorithm values in the model and view
    m_model->getWorld()->setOptions(
        m_controller->getOptions()
    );
    m_view->setMouseAlgorithmAndOptions(
        m_controller->getMouseAlgorithm(),
        m_controller->getOptions()
    );

    // Initialize the tile text, now that the options have been set
    m_view->initTileGraphicText();

    // Lastly, we need to populate the graphics buffers with maze information,
    // but only after we've initialized the tile graphic text
    m_view->getMazeGraphic()->draw();

    // Start the physics loop
    std::thread physicsThread([]() {
        m_model->getWorld()->simulate();
    });

    // Start the solving loop
    std::thread solvingThread([]() {

        // If the maze is invalid, don't let the algo do anything
        if (!m_model->getMaze()->isValidMaze()) {
            return;
        }

        // Wait for the window to appear
        SimUtilities::sleep(Seconds(P()->glutInitDuration()));

        // Begin execution of the mouse algorithm
        m_controller->getMouseAlgorithm()->solve(
            m_model->getMaze()->getWidth(),
            m_model->getMaze()->getHeight(),
            m_model->getMaze()->isOfficialMaze(),
            DIRECTION_TO_CHAR.at(m_model->getMouse()->getCurrentDiscretizedRotation()),
            m_controller->getMouseInterface());
    });

    // Start the graphics loop
    glutMainLoop();
}

} // namespace sim
