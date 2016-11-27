#pragma once

#include <QObject>
#include <QString>
#include <QThread>

#include "DynamicMouseAlgorithmOptions.h"
#include "Model.h"
#include "StaticMouseAlgorithmOptions.h"
#include "View.h"
#include "Worker.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:

    Controller(Model* model, View* view);

    // TODO: MACK
    // ~Controller() {
    //     workerThread.quit();
    //     workerThread.wait();
    // }

    // TODO: MACK - pass these to the worker
    StaticMouseAlgorithmOptions getStaticOptions();
    DynamicMouseAlgorithmOptions getDynamicOptions();

    // TODO: MACK - should be private
    // The algorithm options
    StaticMouseAlgorithmOptions m_staticOptions;
    DynamicMouseAlgorithmOptions m_dynamicOptions;

private:

    // TODO: MACK - make this thread more visible
    Worker* m_worker;
    QThread m_workerThread;

};

} // namespace mms
