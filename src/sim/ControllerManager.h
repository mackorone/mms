#pragma once

#include <QObject>
#include <QThread>
#include <QVector>

#include "Lens.h"
#include "Model.h"
#include "Controller.h"

namespace mms {

class ControllerManager : public QObject {

    Q_OBJECT

public:

    ControllerManager(Model* model, Lens* lens);
    Controller* spawnMouseAlgo(const QString& mouseAlgorithm);

private:

    // The model and lens, which we pass to each controller
    Model* m_model;
    Lens* m_lens;

    // A list of controllers and the threads they're running on
    QVector<QPair<Controller*, QThread*>> m_controllers;

};

} // namespace mms
