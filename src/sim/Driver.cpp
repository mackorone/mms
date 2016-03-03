#include "Driver.h"
#include "Assert.h"
#include "Logging.h"
#include "OnlyExecuteOnce.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include <thread>

namespace sim {

// Definition of the static variables for linking
Model* Driver::m_model;
View* Driver::m_view;
Controller* Driver::m_controller;

void Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    ONLY_EXECUTE_ONCE;

    // First, determine the start time of the program
    double startTime = SimUtilities::getHighResTime();
    // TODO: MACK - consolidate these two

    // Then, determine the runId (just datetime, for now)
    std::string runId = SimUtilities::getDateTime();

    // Then, initiliaze logging (before calling P() or S())
    Logging::initialize(startTime, runId);

    // Initialize the State object in order to:
    // 1) Set the runId
    // 2) Avoid a race condition (between threads)
    // 3) Initialize the Param object
    S()->setRunId(runId);

    // Remove any excessive archived runs
    SimUtilities::removeExcessArchivedRuns();

    // Initialize the model, view, and controller
    m_model = new Model();
    m_view = new View(m_model, argc, argv, {
        []() {
            m_view->refresh();
        },
        [](int width, int height) {
            m_view->updateWindowSize(width, height);
        },
        [](unsigned char key, int x, int y) {
            m_controller->keyPress(key, x, y);
        },
        [](int key, int x, int y) {
            m_controller->specialKeyPress(key, x, y);
        },
        [](int key, int x, int y) {
            m_controller->specialKeyRelease(key, x, y);
        }
    });
    m_controller = new Controller(m_model, m_view);

    // Lastly, we need to populate the graphics buffers with maze information,
    // but only after we've initialized the controller, since the controller
    // initializes tile text
    m_view->getMazeGraphic()->draw();

    // Start the physics loop
    std::thread physicsThread([]() {
        m_controller->getWorld()->simulate();
    });

    // Start the solving loop
    std::thread solvingThread([]() {

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
