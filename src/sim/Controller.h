#pragma once

#include <QObject>
#include <QThread>
#include <QList>

#include "Model.h"
#include "View.h"
#include "Worker.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:

    Controller(Model* model, View* view);
    void spawnMouseAlgo();

private:

    // The model and view, which we pass to each of the workers
    Model* m_model;
    View* m_view;

    // A list of workers and the threads they're running on
    QList<QPair<Worker*, QThread*>> m_controllers;

};

} // namespace mms
