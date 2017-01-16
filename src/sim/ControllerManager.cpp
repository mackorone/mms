#include "ControllerManager.h"

#include "Model.h"

namespace mms {

ControllerManager::ControllerManager(const Maze* maze, MainWindow* window) :
    m_maze(maze),
    m_window(window) {
}

void ControllerManager::spawnMouseAlgo(const QString& mouseAlgorithm) {

    // Generate the mouse, lens, and controller
    Mouse* mouse = new Mouse(m_maze);
    Lens* lens = new Lens(m_maze, mouse);
    Controller* controller = new Controller(m_maze, mouse, lens, mouseAlgorithm);
    MLC mlc = {mouse, lens, controller};

    // Configures the window to listen for build and run stdout,
    // as forwarded by the controller, and adds a map to the UI
    m_window->newMLC(mlc);

    // The thread on which the controller will execute
    QThread* thread = new QThread();
    m_controllers.append({mlc, thread});

    // We need to actually spawn the QProcess (i.e., m_process = new
    // QProcess()) in the separate thread, hence why this is asynch
    connect(thread, &QThread::started, controller, [=](){
        // We need to add the mouse to the world *after* the the controller is
        // initialized (thus ensuring that tile fog is cleared automatically),
        // but *before* we actually start the algorithm (lest the mouse
        // position/orientation not be updated properly during the beginning of
        // the mouse algo's execution)
        controller->init();
        Model::get()->addMouse("", mouse);
        controller->start();
    });
    controller->moveToThread(thread);
	thread->start();
}

} // namespace mms
