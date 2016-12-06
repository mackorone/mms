#include "ControllerManager.h"

namespace mms {

ControllerManager::ControllerManager(Model* model, Lens* lens) :
    m_model(model),
    m_lens(lens) {
}

void ControllerManager::spawnMouseAlgo(const QString& mouseAlgorithm) {

    Controller* controller = new Controller(m_model, m_lens, mouseAlgorithm);
    QThread* thread = new QThread();
    m_controllers.append({controller, thread});

    connect(thread, &QThread::started, controller, &Controller::init);
    controller->moveToThread(thread);
	thread->start();
}

} // namespace mms
