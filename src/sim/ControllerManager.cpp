#include "ControllerManager.h"

namespace mms {

ControllerManager::ControllerManager(
    const World* world,
    const Maze* maze,
    Mouse* mouse,
    Lens* lens) :
    m_world(world),
    m_maze(maze),
    m_mouse(mouse),
    m_lens(lens) {
}

Controller* ControllerManager::spawnMouseAlgo(const QString& mouseAlgorithm) {

    Controller* controller =
        new Controller(m_world, m_maze, m_mouse, m_lens, mouseAlgorithm);
    QThread* thread = new QThread();
    m_controllers.append({controller, thread});

    // We need to spawn the QProcess in the separate thread, hence init
    connect(thread, &QThread::started, controller, &Controller::init);
    controller->moveToThread(thread);
	thread->start();

    return controller;
}

} // namespace mms
