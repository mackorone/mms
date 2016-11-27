#include "Controller.h"

namespace mms {

Controller::Controller(Model* model, View* view) {
    m_worker = new Worker(model, view, this);
    m_worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::started, m_worker, &Worker::init);
	m_workerThread.start();
}

StaticMouseAlgorithmOptions Controller::getStaticOptions() {
    // The Controller object is the source of truth for the static options
    return m_staticOptions;
}

DynamicMouseAlgorithmOptions Controller::getDynamicOptions() {
    // The Controller object is the source of truth for the dynamic options
    return m_dynamicOptions;
}

} // namespace mms
