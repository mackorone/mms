#include "ControllerManager.h"

namespace mms {

ControllerManager::ControllerManager(Model* model, View* view) :
        m_model(model),
        m_view(view) {
}

void ControllerManager::spawnMouseAlgo() {

    Worker* worker = new Worker(m_model, m_view, this);
    QThread* thread = new QThread();
    m_controllers.append({worker, thread});

    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &Worker::init);
	thread->start();
}

} // namespace mms
